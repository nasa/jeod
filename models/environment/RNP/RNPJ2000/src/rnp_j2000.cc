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
 * @file models/environment/RNP/RNPJ2000/src/rnp_j2000.cc
 * Implementation for RNPJ2000
 */

/*******************************************************************************

Purpose:
  ()

Reference:
      (((Mulcihy, David D. and Bond, Victor R.)
        (The RNP Routine for the Standard Epoch J2000)
        (NASA:JSC-24574) (NASA Engineering Directorate, Navigation, Control,
         and Aeronautics Division) (September 1990) (--)))

Assumptions and limitations:
  ((Earth specific))

Class:
  (RNPJ2000)

Library dependencies:
  ((rnp_j2000.cc)
   (nutation_j2000.cc)
   (polar_motion_j2000.cc)
   (precession_j2000.cc)
   (rotation_j2000.cc)
   (environment/RNP/GenericRNP/src/RNP_messages.cc)
   (environment/RNP/GenericRNP/src/planet_rnp.cc)
   (environment/time/src/time_tt.cc)
   (environment/time/src/time_ut1.cc)
   (environment/time/src/time_gmst.cc))

 

*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/time/include/time_manager.hh"
#include "environment/time/include/time_tt.hh"
#include "environment/time/include/time_ut1.hh"
#include "environment/time/include/time_gmst.hh"
#include "environment/time/include/time_dyn.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "../include/rnp_j2000.hh"


//! Namespace jeod
namespace jeod {

/**
 * default constructor. Initialize all data
 */
RNPJ2000::RNPJ2000 (
   void) :
   internal_name("RNPJ2000"),
   gmst_ptr(nullptr),
   time_dyn_ptr(nullptr),
   last_updated_time_full(0.0),
   never_updated_full(true),
   last_updated_time_rotational(0.0),
   never_updated_rotational(true)
{

   // Assign pointer for polymorphic functionality.
   nutation     = &this->NJ2000;
   precession   = &this->PJ2000;
   polar_motion = &this->PMJ2000;
   rotation     = &this->RJ2000;

}

/**
 * Destructor
 */
RNPJ2000::~RNPJ2000 (
   void)
{
}

/**
 * Apply the various options, including fidelity of RNP and if
 * polar motion is enabled or not, then call the base class
 * initializer. Note that this function will still run
 * even if the active flag is set to false. Additionally,
 * note that this function does not update the rotation
 * matrix between inertial and planet fixed. It only
 * sets up the object to work with the DynManager.
 * update_rnp must be called to update the
 * RNP matrix. This function will also cache of TimeGMST
 * from the given TimeManager
 * \param[in,out] dyn_manager DynManager where the planet attitude to be updated is contained
 */
void
RNPJ2000::initialize (
   DynManager& dyn_manager)
{

   RJ2000.planet_rotational_velocity = planet_omega;

   if ((rnp_type == RotationOnly) || (rnp_type == ConstantNP)) {
      RJ2000.use_full_rnp = false;
   }


   if (rnp_type == FullRNP) {
      RJ2000.nutation = &NJ2000;
   }




   PlanetRNP::initialize (dyn_manager);
}

// /*******************************************************************************
//  Function: update_rnp(TimeTT&, TimeGMST& gmst_ptr, TimeUT1&)
//  Purpose: (Update all present components of the RNP, combine them and send
//            them to the correct planet from the dyn manager in initialize)
//  Class: (environment)
// *******************************************************************************/
/**
 * Updates the complete RNP and supplies it to the Dynamics Manager.
 * \param[in] time_tt The current time in the TT time standard
 * \param[in] time_gmst current time in the GMST time standard\n Units: The
 * \param[in] time_ut1 current time in the UT1 time standard\n Units: The
 */
//  Function: update_rnp(TimeTT&, TimeGMST& gmst_ptr, TimeUT1&)
//  Purpose: (Update all present components of the RNP, combine them and send
//            them to the correct planet from the dyn manager in initialize)
//  Class: (environment)
// *******************************************************************************/
/*
  Purpose:
    (Updates the complete RNP and supplies it to the Dynamics Manager.)
 */
void
RNPJ2000::update_rnp (
   const TimeTT& time_tt,
   TimeGMST& time_gmst,
   const TimeUT1& time_ut1)
{

   // check if active and get out if not
   if(!active) {
      return;
   }

   // If the DynTime pointer has not been filled out yet, then we need
   // to go and get that.

   if(time_dyn_ptr == nullptr) {
      get_dyn_time_ptr(time_gmst);
   }

   // If the TimeGMST pointer hasn't been cached off yet, do that.
   if(gmst_ptr == nullptr) {
      gmst_ptr = &time_gmst;
   }

   // check if it even needs to be updated. If it has been previously
   // updated, and if the time has not been changed, then we don't
   // need to update.
   if(!never_updated_full &&
       Numerical::compare_exact(last_updated_time_full,time_dyn_ptr->seconds)) {
       return;
   }

   // If we are this far, then we are updating! and can set our update flags

   // This is the full RNP update, so it will ALSO get the rotational update.
   // so set all flags to be correct.
   never_updated_full = false;
   never_updated_rotational = false;
   last_updated_time_full = last_updated_time_rotational = time_dyn_ptr->seconds;

   double time = 0.0;

   // rotation needs seconds since J2000
   time = time_gmst.seconds;

   if (enable_polar == true) {

      // polar motion needs UT1 based modified julian date
      polar_motion->update_time (
         time_ut1.trunc_julian_time + (2440000.5 - 2400000.5));
   }

   if (rotation != nullptr) {
      if (rnp_type == FullRNP) {
         rotation->update_time (time);
      }
      else {
         time = time_gmst.time_manager->dyn_time.seconds;
         rotation->update_time (time);
      }
   }
   /* compute the fraction of a julian century from the standard
   epoch J2000 to julian_date.
   To compute the precession and nutation matrices. Ported from
   Jeod 1.52 */
   // time = ( time_tt.trunc_julian_time - 2451545.0 + 2440000.5 ) / 36525.0 ;
   time = (time_tt.trunc_julian_time + (2440000.5 - 2451545.0)) / 36525.0;
   if (rnp_type == FullRNP) {
      nutation->update_time (time);
      precession->update_time (time);
   }

   // update the timestamp of the controlled reference frame.
   planet->pfix.set_timestamp(time_dyn_ptr->seconds);

   PlanetRNP::update_rnp();

   return;
}

// /*******************************************************************************
//  Function: update_axial_rotation(TimeUT1&)
//  Purpose: (Same as update_rnp, but only updates axial rotation part)
//  Class: (derivative)
// *******************************************************************************/
/**
 * Updates the axial rotation portion of RNP
 * and supplies the entire RNP to the Dynamics Manager.
 * \param[in] time_gmst current time in the GMST time standard\n Units: The
 */
//  Function: update_axial_rotation(TimeUT1&)
//  Purpose: (Same as update_rnp, but only updates axial rotation part)
//  Class: (derivative)
// *******************************************************************************/
/*
  Purpose:
    (Updates the axial rotation portion of RNP
     and supplies the entire RNP to the Dynamics Manager.)
 */
void
RNPJ2000::update_axial_rotation (
   TimeGMST& time_gmst)
{

   // check if active and escape if not

   if(!active) {
      return;
   }

   // If the DynTime pointer has not been filled out yet, then we need
   // to go and get that.

   if(time_dyn_ptr == nullptr) {
      get_dyn_time_ptr(time_gmst);
   }

   // check if it even needs to be updated. If it has been previously
   // updated, and if the time has not been changed, then we don't
   // need to update.
   if(!never_updated_rotational &&
       Numerical::compare_exact(last_updated_time_rotational,time_dyn_ptr->seconds)) {
       return;
   }

   // If we are this far, then we are updating! and can set our update flags

   // This is the rotational only update, so only set the rotational parts
   never_updated_rotational = false;
   last_updated_time_rotational = time_dyn_ptr->seconds;

   double time = 0.0;

   time = time_gmst.seconds;
   if (rotation != nullptr) {
     if (rnp_type == FullRNP) {
         rotation->update_time (time);
      }
      else {
         time = time_gmst.time_manager->dyn_time.seconds;
         rotation->update_time (time);
      }
   }

   // update the timestamp of the controlled reference frame.
   planet->pfix.set_timestamp(time_dyn_ptr->seconds);

   PlanetRNP::update_axial_rotation();

   return;
}

double RNPJ2000::timestamp() const {
   return last_updated_time_rotational;
}

const char* RNPJ2000::get_name() const {
   return internal_name.c_str();
}


// /*******************************************************************************
//  Function: ephem_update()
//  Purpose: (The RNPJ2000 implementation of the pure virtual EphemerisInterface
//            function. Internally calls update_axial_rotation. update_rnp
//            must be called prior to this function being called, or RNP
//            will not be correctly updated.)
//
// *******************************************************************************/
void RNPJ2000::ephem_update() {

   if(active && orient_interface.is_active()) {
      if(gmst_ptr == nullptr) {
         MessageHandler::inform (
            __FILE__, __LINE__, RNPMessages::setup_error,
            "RNPJ2000::ephem_update was called without a valid "
             "gmst_ptr being set. It is normally set in the "
             "RNPJ2000::update_rnp function. RNP was not updated.\n");
         return;
      }

      update_axial_rotation(*gmst_ptr);
   }
}

// /*******************************************************************************
//  Function: get_dyn_time_ptr(TimeGMST&)
//  Purpose: (Accesses the TimeManager through the given gmst, and
//            uses is to get the TimeDyn for the simulation)
// *******************************************************************************/
void RNPJ2000::get_dyn_time_ptr( // RETURN: -- none.
   TimeGMST& gmst)  /* In: -- The current time in the GMST time standard */
{

   // If we already have a time_dyn_ptr, then this must be a miscall, and we
   // can just return.
   if(time_dyn_ptr != nullptr) {
      return;
   }

   time_dyn_ptr = dynamic_cast<TimeDyn*> (gmst.time_manager->get_time_ptr("Dyn"));

   if(time_dyn_ptr == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::setup_error,
         "The TimeManager associated with the given TimeGMST "
         "did not contain a TimeDyn object. This is "
         "a fatal error.\n");
      // should never be reached, but for completeness:
      return;
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
