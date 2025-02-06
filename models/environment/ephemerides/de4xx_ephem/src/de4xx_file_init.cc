/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup De4xxEphem
 * @{
 *
 * @file models/environment/ephemerides/de4xx_ephem/src/de4xx_file_init.cc
 * Define De4xx initialization methods.
 */

/*******************************************************************************

Purpose:
   ()

Library dependency:
   ((de4xx_file_init.o)
    (de4xx_file.o)
    (environment/ephemerides/ephem_interface/ephem_messages.o)
    (utils/message/message_handler.o))



*******************************************************************************/


// System includes
#include <cerrno>            // errno
#include <climits>           // CHAR_BIT
#include <cmath>             // floor
#include <cstddef>           // NULL
#include <cstring>           // strcmp, strerrno
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>          // fstat
#include <dlfcn.h>


// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/de4xx_file.hh"



//! Namespace jeod
namespace jeod {

static double l1_point (double b1b2_mass_ratio);


/* External functions */


/**
 * Pre-initialize a DE4xxFile instance.
 */


/*******************************************************************************
Purpose: (Pre-initialize a DE4xxFile instance.)
*******************************************************************************/
void
De4xxFile::pre_initialize (
   void)
{
   // Sanity check: Already initialized: simulation programmer error.
   if (io.file != NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::internal_error,
         "Ephemerides model already initialized.");

      // Not reached
      return;
   }

   // Open the ephemeris file.
   open (); // flawfinder: ignore

   // Clear dlerror
   char * dlError = dlerror();

   // Grab the first segment data segment symbol as a starting point
   io.recno = 0;
   io.segment_index = 0;
   io.segment_recno = 0;
   io.coeffs_segment_starting_addr = (double *)dlsym(io.file, "segment_coeffs_0");
   if (io.coeffs_segment_starting_addr == NULL) {
      dlError = dlerror();
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::file_error,
         "Error obtaining ephemeris file symbol 'segment_coeffs_0' from '%s' for input: %s",
         file_spec.pathname.c_str(), dlError);

      // Not reached
      return;
   }
   io.current_record_starting_addr = io.coeffs_segment_starting_addr;
   coef.coef = io.coeffs_segment_starting_addr;

//   capture_mem_stats();

   // Sanity check: The ephemeris model number constant from the symbol file
   // This should match the DE number specified in the input structure.
   uint32_t file_denum = static_cast<int>(io.metaData->de_constants[De4xxBase::De4xx_Const_DENUM]);
   if (file_denum != file_spec.denum) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::garbage_file,
         "Unable to parse ephemeris header, found DE# = %d", file_denum);

      // Not reached
      return;
   }

   // Extract the indices of each data item in the ephemeris file and
   // determine item availability.
   io.max_terms = 0;
   for (uint32_t ii = 0; ii < io.metaData->number_file_items; ii++) {
      EphemerisDataItemMeta & itemData = io.itemData[ii];
      De4xxFileItem & item_ii = item[ii];

      item_ii.item_idx = ii;
      if (itemData.offset > 0)
      {
          if (io.max_terms < itemData.nterms)
          {
              io.max_terms = itemData.nterms;
          }
          item_ii.avail = true;
      }
      else
      {
        item_ii.avail = false;
      }
   }

   // Any remaining indices in the item array should be set to null and unavailable
   for(int ii = io.metaData->number_file_items; ii < De4xxBase::De4xx_File_MaxEntries; ++ii) {
       De4xxFileItem * item_ii = &(item[ii]);
       item_ii->item_idx = 0;
       item_ii->avail = false;
   }

   // Handle segment overlap case by subtracting from the number of records in a segment if exceeding
   // the next segment's start_epoch
   io.total_num_recs = 0;
   for (uint32_t ii = 0; ii < io.metaData->number_segments; ++ii)
   {
       EphemerisDataSegmentMeta &segData = io.segmentData[ii];
       if ((ii+1) < io.metaData->number_segments)
       {
           EphemerisDataSegmentMeta &segDataPrev = io.segmentData[ii+1];
           if (segData.stop_epoch > segDataPrev.start_epoch )
           {
               --segData.num_recs;
           }
       }
       io.total_num_recs += segData.num_recs;
   }
}


/**
 * Initialize a DE4xxFile instance.
 * \param[in] epoch_time Julian date\n Units: day
 * \param[in] del_day Days from epoch\n Units: day
 * \param[in] time_offset Terrestrial Time offet\n Units: s
 * \param[in] init_time Seconds from epoch\n Units: s
 */
void
De4xxFile::initialize (
   double epoch_time,
   double del_day,
   double time_offset,
   double init_time)
{
   double day_offset;
   double f;


   // Perform items common to the initial open / and restart re-open.
   pre_initialize();

   // Allocate buffers for reading and interpreting ephemeris file records.
   coef.chebypoly  = JEOD_ALLOC_PRIM_ARRAY (io.max_terms, double);
   coef.chebyderiv = JEOD_ALLOC_PRIM_ARRAY (io.max_terms, double);

   // Grab important constants from the symbol file
   header.au            = io.metaData->de_constants[De4xxBase::De4xx_Const_AU];
   header.em_mass_ratio = io.metaData->de_constants[De4xxBase::De4xx_Const_EMRAT];
   header.vlight        = io.metaData->de_constants[De4xxBase::De4xx_Const_CLIGHT];

   header.gmbody[De4xxBase::De4xx_Ephem_Mercury] = io.metaData->de_constants[De4xxBase::De4xx_Const_GM1];
   header.gmbody[De4xxBase::De4xx_Ephem_Venus]   = io.metaData->de_constants[De4xxBase::De4xx_Const_GM2];
   header.gmbody[De4xxBase::De4xx_Ephem_EMbary]  = io.metaData->de_constants[De4xxBase::De4xx_Const_GMB];
   header.gmbody[De4xxBase::De4xx_Ephem_Mars]    = io.metaData->de_constants[De4xxBase::De4xx_Const_GM4];
   header.gmbody[De4xxBase::De4xx_Ephem_Jupiter] = io.metaData->de_constants[De4xxBase::De4xx_Const_GM5];
   header.gmbody[De4xxBase::De4xx_Ephem_Saturn]  = io.metaData->de_constants[De4xxBase::De4xx_Const_GM6];
   header.gmbody[De4xxBase::De4xx_Ephem_Uranus]  = io.metaData->de_constants[De4xxBase::De4xx_Const_GM7];
   header.gmbody[De4xxBase::De4xx_Ephem_Neptune] = io.metaData->de_constants[De4xxBase::De4xx_Const_GM8];
   header.gmbody[De4xxBase::De4xx_Ephem_Pluto]   = io.metaData->de_constants[De4xxBase::De4xx_Const_GM9];
   header.gmbody[De4xxBase::De4xx_Ephem_Sun]     = io.metaData->de_constants[De4xxBase::De4xx_Const_GMS];


   // Compute derived constants.
   header.be_em_dist_ratio = 1.0 / (1.0 + header.em_mass_ratio);
   header.bm_em_dist_ratio = header.be_em_dist_ratio *
                             header.em_mass_ratio;
   header.e1_em_dist_ratio = l1_point (header.em_mass_ratio);
   header.b1_em_dist_ratio = header.e1_em_dist_ratio -
                             header.be_em_dist_ratio;

   // Compute the Moon and Earth gravitational constants.
   header.gmbody[De4xxBase::De4xx_Ephem_Moon]  =
      header.gmbody[De4xxBase::De4xx_Ephem_EMbary] * header.be_em_dist_ratio;
   header.gmbody[De4xxBase::De4xx_Ephem_Earth] =
      header.gmbody[De4xxBase::De4xx_Ephem_Moon] * header.em_mass_ratio;

   // Convert distance and velocity constants from km, km/sec to MKS units.
   header.au     *= 1e3;
   header.vlight *= 1e3;

   // Convert the gravitational constants from AU^3/day^2 to MKS units.
   f = (header.au * header.au * header.au) / (86400.0 * 86400.0);
   for ( uint32_t ii = 0;
         ii < De4xxBase::number_grav_models(file_spec.denum);
         ii++)
   {
      header.gmbody[ii] *= f;
   }

   // Save the reference time.
   del_day += (time_offset + init_time) / 86400.0;
   ref_time.epoch_date  = std::floor (epoch_time + del_day) + 0.5;
   ref_time.fdate       = (epoch_time - ref_time.epoch_date) + del_day;
   day_offset           = std::floor (ref_time.fdate);
   ref_time.epoch_date += day_offset;
   ref_time.fdate      -= day_offset;
   ref_time.init_time   = init_time;


   // Compute the reference block.
   day_offset = (ref_time.epoch_date - io.segmentData[0].start_epoch) + ref_time.fdate;
   ref_time.block_no = day_offset / io.metaData->delta_epoch;

   // Sanity check: Ensure the timepoint of interest is in the database.
   if ((ref_time.block_no < 0) ||
           (ref_time.block_no >= static_cast<double>(io.total_num_recs))) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::time_not_in_range,
         "Time out of range");

      // Not reached
      return;
   }


   // The item state data is zero-filled by default.
   // Force an update by setting the time to an invalid value.
   update_time = init_time - 1.0;
}

/**
 * Calculate the location of the L1 point as a ratio.
 * @return Ratio of body1 to L1-point distance to body1 to body2 distance
 * \param[in] b1b2_mass_ratio Body1 to body2 mass ratio
 */
static double
l1_point (
   double   b1b2_mass_ratio)
{
   double k, kp1, k3p2, k3p1;
   double f, df;
   double z, dz;

   // The ratio of the body1-L1 distance to the body1-body2 distance 'z'
   // is the real solution to the fifth-order equation
   //    (k+1)*z^5 - (3k+2)*z^4 + (3k+1)*z^3 - z^2 + 2z - 1 = 0
   // where 'k' is the body2-to-body1 mass ratio.
   // Solve for z via Newton-Raphson.
   k = 1.0 / b1b2_mass_ratio;
   kp1  = k + 1.0;
   k3p1 = 3.0*k + 1.0;
   k3p2 = 3.0*k + 2.0;
   z = 0.8;
   dz = 1.0;
   while ((dz > 1e-15) || (dz < -1e-15)) {
      f  = -1.0 + z*(2.0 - z*(1.0 - z*(k3p1 - z*(k3p2 - z*kp1))));
      df =  2.0 - z*(2.0 - z*(3.0*k3p1 - z*(4.0*k3p2 - z*5.0*kp1)));
      dz = -f / df;
      z += dz;
   }

   return z;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
