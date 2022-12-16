/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/gravity_integ_frame.cc
 * Define member functions for the GravityIntegFrame class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((gravity_integ_frame.o)
   (environment/ephemerides/ephem_interface/ephem_ref_frame.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/gravity_integ_frame.hh"


//! Namespace jeod
namespace jeod {

/**
 * GravityIntegFrame constructor.
 */
GravityIntegFrame::GravityIntegFrame (
   void)
{
   ref_frame     = NULL;
   is_third_body = false;
   Vector3::initialize (pos);
   Vector3::initialize (accel);
   time = 9e99;
}


/**
 * GravityIntegFrame destructor.
 */
GravityIntegFrame::~GravityIntegFrame (
   void)
{
   ; // Void, for now
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
