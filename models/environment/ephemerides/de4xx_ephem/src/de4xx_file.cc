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
 * @file models/environment/ephemerides/de4xx_ephem/src/de4xx_file.cc
 * This file defines several utility functions used to read a binary JPL DE405
 * ephemeris file. The functions are
 *
 *   open        - Open an ephemeris file for input
 *   close       - Close a previously open ephemeris file
 *   read_record - Read a record from the ephemeris file
 *   get_string  - Get a string from the current data record
 *   get_int     - Get integer array from the current data record
 *   get_double  - Get double array from the current data record
 *
 * NOTA BENE -- The functions defined in this file are intended for use by the
 * top-level ephemeris functions only.
 */

/*******************************************************************************

Purpose:
   ()

Reference:
   (((None)))

Assumptions and limitations:
   ((User correctly specified byteswapping in the EphemeridesIn structure)
    (Local machine is either big-endian or little-endian)
    (Errors are fatal))

Library dependency:
   ((de4xx_file.o)
    (de4xx_file_init.o)
    (de4xx_file_update.o)
    (environment/ephemerides/ephem_interface/ephem_messages.o)
    (utils/sim_interface/memory_interface.o)
    (utils/message/message_handler.o))


*******************************************************************************/


#define __STDC_LIMIT_MACROS
// System includes
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <limits>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>

#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>


// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/de4xx_file.hh"



//! Namespace jeod
namespace jeod {

/**
 * Construct a De4xxFileSpec object.
 */
De4xxFileSpec::De4xxFileSpec (
   void)
:
   denum(405)
{

    ephem_file_dir = "build/de4xx_lib";
    set_model_number(405);
}


void De4xxFileSpec::set_model_number(int denum_in)
{
    denum = denum_in;
    ephem_file_name = "libde" + std::to_string(denum) +".so";
    pathname = ephem_file_dir + "/" + ephem_file_name;
}


/**
 * Construct a De4xxFileIO object.
 */
De4xxFileIO::De4xxFileIO (
   void)
:
   metaData(NULL),
   itemData(NULL),
   segmentData(NULL),
   coeffs_segment_starting_addr(NULL),
   current_record_starting_addr(NULL),
   recno(std::numeric_limits<int>::max()),
   segment_index(0),
   segment_recno(0),
   total_num_recs(0),
   max_terms(0),
   file(0x0)
{
   ; // Empty
}


/**
 * Construct a De4xxFileHeader object.
 */
De4xxFileHeader::De4xxFileHeader (
   void)
:
   au(0.0),
   vlight(0.0),
   em_mass_ratio(0.0),
   be_em_dist_ratio(0.0),
   bm_em_dist_ratio(0.0),
   e1_em_dist_ratio(0.0),
   b1_em_dist_ratio(0.0)
{
   // Allocate max
   gmbody = JEOD_ALLOC_PRIM_ARRAY(De4xxBase::number_grav_models(999), double );
   for (uint32_t ii = 0; ii < De4xxBase::number_grav_models(999); ++ii) {
      gmbody[ii] = 0.0;
   }
}

/**
 * Destruct a De4xxFileHeader object.
 */
De4xxFileHeader::~De4xxFileHeader (
   void)
{
   JEOD_DELETE_ARRAY( gmbody );
}


/**
 * Construct a De4xxFileItem object.
 * As most ephemeris file items are position vectors in kilometers,
 * this constructor sets the scale to 1000 and the number of items
 * to three.
 */
De4xxFileItem::De4xxFileItem (
   void)
:
   active(false),
   avail(false),
   item_idx(-1),
   nitems(3),
   pscale(1000.0),
   update_time(-99e99)
{
   // Initialize the state to garbage.
   for (unsigned int ii = 0; ii < 3; ++ii) {
      state[0][ii] = -99e99;
      state[1][ii] = -99e99;
   }
}


/**
 * Construct a De4xxFileRefTime object.
 */
De4xxFileRefTime::De4xxFileRefTime (
   void)
:
   epoch_date(-99e99),
   fdate(-99e99),
   time_offset(-99e99),
   init_time(-99e99),
   block_no(-99e99)
{
   ; // Empty
}


/**
 * Construct a De4xxFileFileCoef object.
 */
De4xxFileCoef::De4xxFileCoef (
   void)
:
//   ncoef(0),
   chebyterms(0),
   chebyx(-99e99),
   chebypoly(NULL),
   chebyderiv(NULL),
   coef(NULL)
{
   ; // Empty
}


/**
 * Construct a De4xxFileRestart object.
 * \param[in,out] in The De4xxFile object
 */
De4xxFileRestart::De4xxFileRestart (
   De4xxFile & in)
:
   de4xx_file(in)
{
   ; // Empty
}


/**
 * Destroy a De4xxFileRestart object.
 */
De4xxFileRestart::~De4xxFileRestart (
   void)
{
   ; // Empty
}


/**
 * Reopen the De4xx file for a restart.
 */
void
De4xxFileRestart::simple_restore (
   void)
{
   de4xx_file.reopen();
}


/**
 * Construct the JPL ephemeris file.
 */
De4xxFile::De4xxFile (
   void)
:
   file_spec(),
   header(),
   io(),
   ref_time(),
   coef(),
   restart(*this),
   update_time(-99e99)
{
   // Register the file restart mechanism.
   JEOD_REGISTER_CLASS (De4xxFile);
   JEOD_REGISTER_CHECKPOINTABLE (this, restart);

   item = JEOD_ALLOC_CLASS_ARRAY( De4xxBase::De4xx_File_MaxEntries, De4xxFileItem );

   // Override the defaults for the nutation and libration items.

   // Nutation data have two coefficient sets per item
   // and are represented in radians (scale factor = 1).
   item[De4xxBase::De4xx_File_ENutation].pscale = 1.0;
   item[De4xxBase::De4xx_File_ENutation].nitems = 2;

   // "Libration" data have three coefficient sets per item
   // and are represented in radians (scale factor = 1).
   item[De4xxBase::De4xx_File_LLibration].pscale = 1.0;
   item[De4xxBase::De4xx_File_LLibration].nitems = 3;

   // "tt_tdb" data has a single offset per item
   // and is represented in seconds (scale factor = 1).
   item[De4xxBase::De4xx_File_tt_tdb].pscale = 1.0;
   item[De4xxBase::De4xx_File_tt_tdb].nitems = 1;
}



/**
 * Destroy the JPL ephemeris file.
 */
De4xxFile::~De4xxFile (
   void)
{
   JEOD_DEREGISTER_CHECKPOINTABLE (this, restart);
   JEOD_DELETE_ARRAY(item);
   close ();
}



/**
 * Shutdown the JPL ephemeris file.
 */
void
De4xxFile::shutdown (
   void)
{
   close ();
}


/**
 * Open the JPL ephemeris file.
 *
 * \par Assumptions and Limitations
 *  - Errors are fatal
 * @return Void
 */
void
De4xxFile::open ( // flawfinder: ignore
   void)
{
   // Clear dlerror
   char * dlError = dlerror();

//   capture_mem_stats();

   // Use dynamic loader to load the file but only symbols as needed
   io.file = dlopen(file_spec.pathname.c_str(), RTLD_LAZY | RTLD_LOCAL);
   if (io.file == NULL) {
      dlError = dlerror();
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::file_error,
         "Error opening ephemeris file '%s' for input: %s",
         file_spec.pathname.c_str(), dlError);

      // Not reached
      return;
   } else  {
       MessageHandler::debug(
          __FILE__, __LINE__, EphemeridesMessages::debug,
          "Loading ephemeris data '%s' in '%s'",
          file_spec.ephem_file_name.c_str(),
          file_spec.ephem_file_dir.c_str());
   }

   io.metaData = (jeod::EphemerisDataSetMeta *)dlsym(io.file, "metaData");
   if (io.metaData == NULL) {
      dlError = dlerror();
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::file_error,
         "Error obtaining ephemeris file symbol 'metaData' from '%s' for input: %s",
         file_spec.pathname.c_str(), dlError);

      // Not reached
      return;
   }

   if(io.metaData->number_file_items > De4xxBase::De4xx_File_MaxEntries)
   {
       MessageHandler::fail (
          __FILE__, __LINE__, EphemeridesMessages::file_error,
          "Error loading ephemeris data from '%s' for input: %s Max: %d Actual: %d",
          file_spec.pathname.c_str(), "Exceeds maximum ephemeris entries",
          De4xxBase::De4xx_File_MaxEntries, io.metaData->number_file_items);

       // Not reached
       return;

   }

   io.itemData = (jeod::EphemerisDataItemMeta *)dlsym(io.file, "itemData");
   if (io.itemData == NULL) {
      dlError = dlerror();
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::file_error,
         "Error obtaining ephemeris file symbol 'itemData' from '%s' for input: %s",
         file_spec.pathname.c_str(), dlError);

      // Not reached
      return;
   }

   io.segmentData = (jeod::EphemerisDataSegmentMeta *)dlsym(io.file, "segmentData");
   if (io.segmentData == NULL) {
      dlError = dlerror();
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::file_error,
         "Error obtaining ephemeris file symbol 'segmentData' from '%s' for input: %s",
         file_spec.pathname.c_str(), dlError);

      // Not reached
      return;
   }

//   capture_mem_stats();
}


/**
 * Open the JPL ephemeris file on restart.
 *
 * \par Assumptions and Limitations
 *  - File spec has been reloaded.
 *  - Data has been allocated
 *  - Errors are fatal
 */
void
De4xxFile::reopen (
   void)
{
   // Close the file if it is already open.
   if (io.file != NULL) {
       dlclose (io.file);
       io.file = NULL;
   }

   // Reopen with the common initialization / restart pre_initialize function.
   pre_initialize ();
}


/**
 * Close the JPL ephemeris file.
 *
 * \par Assumptions and Limitations
 *  - Ephemeris file is open for input
 *  - Errors are fatal
 */
void
De4xxFile::close (
   void)
{
   int rc;

   // Nothing to do if file is already closed.
   if (io.file == NULL) {
      return;
   }

   // Close the file.
   rc = dlclose (io.file);

   // Check for success.
   // NOTE: Failure is a warning here as the close may be the result of an
   // exec_terminate call elsewhere.
   if (rc != 0) {
      MessageHandler::warn (
         __FILE__, __LINE__, EphemeridesMessages::file_error,
         "Error closing ephemeris file: %s",
         std::strerror(errno));
   }

   // Denote that the file is closed.
   io.file  = NULL;
   io.metaData = NULL;

   // Free allocated memory.
   JEOD_DELETE_ARRAY (coef.chebypoly);
   JEOD_DELETE_ARRAY (coef.chebyderiv);
}


/**
 * Check whether the specified time is represented in the JPL ephemeris file.
 *
 * \par Assumptions and Limitations
 *  - Ephemeris file is open for input
 *  - Ephemeris file is blocked per value set in the ephem_file structure
 * @return True if time is in file
 * \param[in] time Time since reference\n Units: s
 */
bool
De4xxFile::time_is_in_range (
   double time)
const
{
   double recno;

   // Compute record number corresponding to this time.
   // Note that this does not quite do the right thing for negative values.
   // This is OK since the minimum value record number is two.
    recno = ref_time.block_no +
               (time - ref_time.init_time) / (86400.0 * io.metaData->delta_epoch);


   // Return an indicator as to whether the input time is in range.
   // Note that the first two data records are out of range.
    return (recno >= 0) &&
           (recno < io.total_num_recs);
}

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

void De4xxFile::capture_mem_stats()
{
    using std::cout;
    using std::endl;

    if( logMemoryStats ) {
        process_mem_usage(vm_usage, resident_set);
        cout << "VM: " << vm_usage << "; RSS: " << resident_set << endl;
    }
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
