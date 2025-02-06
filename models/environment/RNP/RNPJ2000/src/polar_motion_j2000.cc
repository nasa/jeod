/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPJ2000
 * @{
 *
 * @file models/environment/RNP/RNPJ2000/src/polar_motion_j2000.cc
 * Implementation of PolarMotionJ2000
 */

/*******************************************************************************

Purpose:
  ()

Reference:
    (((G.H. Kaplan)
      (U.S. Naval Observatory Vector Astrometry Subroutines)
      (NOVAS FORTRAN vers F2.0) (1 Nov 98)))

Assumptions and limitations:
  ((The matrix produced is currently an approximation, and not completely
    orthonormal.))

Class:
  (PolarMotionJ2000)

Library dependencies:
  ((polar_motion_j2000.cc)
   (polar_motion_j2000_init.cc)
   (environment/RNP/GenericRNP/src/RNP_messages.cc)
   (environment/RNP/GenericRNP/src/planet_rotation.cc)
   (environment/RNP/GenericRNP/src/planet_rotation_init.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc))

 
*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/polar_motion_j2000.hh"
#include "../include/polar_motion_j2000_init.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor. Initializes all data.
 */
PolarMotionJ2000::PolarMotionJ2000 (
   void)
:
   xp(0.0),
   yp(0.0),
   xp_tbl(nullptr),
   yp_tbl(nullptr),
   polar_mjd(nullptr),
   override_table(false),
   last_table_index(0),
   warn_table(false)
{
// empty for now
}

/**
 * Destructor
 */
PolarMotionJ2000::~PolarMotionJ2000 (
   void)
{
   if (xp_tbl != nullptr && JEOD_IS_ALLOCATED(xp_tbl)) {
      JEOD_DELETE_ARRAY (xp_tbl);
      xp_tbl = nullptr;
   }
   if (yp_tbl != nullptr && JEOD_IS_ALLOCATED(yp_tbl)) {
      JEOD_DELETE_ARRAY (yp_tbl);
      yp_tbl = nullptr;
   }
   if (polar_mjd != nullptr && JEOD_IS_ALLOCATED(polar_mjd)) {
      JEOD_DELETE_ARRAY (polar_mjd);
      polar_mjd = nullptr;
   }


}
/**
 * PolarMotionJ2000 specific implementaiton of update_rotation from
 * PlanetRotation
 */
void
PolarMotionJ2000::update_rotation (
   void)
{

   // This implements an interpolation table, based on the time in
   // modified julian date in UT1 being the independent variable and
   // the xp and yp values being the dependent variables
   double time = current_time;
   if (!override_table) {

      unsigned int index_in_table = 0;
      if (time >= polar_mjd[last_table_index]) {

         // Julian date is beyond the table, set to last value
         xp = xp_tbl[last_table_index];
         yp = yp_tbl[last_table_index];

         if (warn_table != true) {
            MessageHandler::warn (
               __FILE__, __LINE__, RNPMessages::polar_motion_table_warning,
               "Julian date is beyond values in polar motion \n tables."
               "The polar motion is being set to\n the last value of the tables.");
            warn_table = true;
         }
      }


      else if (time <= polar_mjd[0]) {

         xp = xp_tbl[0];
         yp = yp_tbl[0];

         if (warn_table != true) {
            MessageHandler::warn (
               __FILE__, __LINE__, RNPMessages::polar_motion_table_warning,
               "Julian date is before values in polar motion \n tables. "
               "The polar motion is being set to\n the first value of the tables.");
            warn_table = true;
         }

      }

      else { // need to interpolate xp and yp out of the tables
         warn_table = false;
         for (unsigned int i = 0; i < last_table_index; ++i) {
            if (time < polar_mjd[i + 1]) {
               index_in_table = i;
               break;
            } // if(time < polar_mjd)
         } // for(unsigned int i = 0)

         xp = xp_tbl[index_in_table] +
              (xp_tbl[index_in_table + 1] - xp_tbl[index_in_table]) *
              ((time - polar_mjd[index_in_table]) /
               (polar_mjd[index_in_table + 1] - polar_mjd[index_in_table]));


         yp = yp_tbl[index_in_table] +
              (yp_tbl[index_in_table + 1] - yp_tbl[index_in_table]) *
              ((time - polar_mjd[index_in_table]) /
               (polar_mjd[index_in_table + 1] - polar_mjd[index_in_table]));

      } // else
   } // if(!override_table)

   /* The original version which used small angle approximations
   has been replaced by the exact trigonometric solution per #713 */
//
//   rotation[0][0] = 1.0;
//   rotation[0][1] = 0.0;
//   rotation[0][2] = -xp;
//
//   rotation[1][0] = 0.0;
//   rotation[1][1] = 1.0;
//   rotation[1][2] = yp;
//
//   rotation[2][0] = xp;
//   rotation[2][1] = -yp;
//   rotation[2][2] = 1.0;

   double cos_xp = cos(xp); double sin_xp = sin(xp);
   double cos_yp = cos(yp); double sin_yp = sin(yp);

   rotation[0][0] = cos_xp;
   rotation[0][1] = 0.0;
   rotation[0][2] = -sin_xp;

   rotation[1][0] = sin_xp * sin_yp;
   rotation[1][1] = cos_yp;
   rotation[1][2] = cos_xp * sin_yp;

   rotation[2][0] = sin_xp * cos_yp;
   rotation[2][1] = -sin_yp;
   rotation[2][2] = cos_xp * cos_yp;


   return;
}

/**
 * Initialize the coefficients of PolarMotionJ2000. init must be of
 * type PolarMotionJ2000Init or a fail message will occur.
 * \param[in] init PolarMotionJ2000Init with needed coefficients
 */
void
PolarMotionJ2000::initialize (
   PlanetRotationInit* init)
{

   PolarMotionJ2000Init* pm_init =
      dynamic_cast<PolarMotionJ2000Init*> (init);

   if (pm_init == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::initialization_error,
         "Init object sent to PolarMotionJ2000 was"
         " not of type PolarMotionJ2000Init");
      return;
   }

   override_table   = pm_init->override_table;
   last_table_index = pm_init->last_table_index;
   xp               = pm_init->xp;
   yp               = pm_init->yp;

   // need to allocate last table index + 1, which will
   // be the size
   xp_tbl    = JEOD_ALLOC_PRIM_ARRAY (last_table_index + 1, double);
   yp_tbl    = JEOD_ALLOC_PRIM_ARRAY (last_table_index + 1, double);
   polar_mjd = JEOD_ALLOC_PRIM_ARRAY (last_table_index + 1, double);

   for (unsigned int ii = 0; ii < last_table_index + 1; ++ii) {

      xp_tbl[ii]    = pm_init->xp_tbl[ii];
      yp_tbl[ii]    = pm_init->yp_tbl[ii];
      polar_mjd[ii] = pm_init->polar_mjd[ii];

   }

   return;
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
