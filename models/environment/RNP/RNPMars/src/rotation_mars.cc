/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPMars
 * @{
 *
 * @file models/environment/RNP/RNPMars/src/rotation_mars.cc
 * Implementation of RotationMars
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((Konopliv, Alex S., et al)
    (A global solution for the Mars static and seasonal gravity, Mars
     orientation, Phobos and Deimos masses, and Mars ephemeris)
    (Icarus, Volume 182, Issue 1, Pages 23-50) (May 2006))
   ((Konopliv, Alex S., et al)
    (Mars high resolution gravity fields from MRO, Mars season gravity, and
     other dynamical parameters)
    (Icarus, Volume 211, Issue 1, Pages 401-428) (January 2011)))

Assumptions and limitations:
  ((Mars specific))

Library dependencies:
  ((rotation_mars.o)
   (environment/RNP/GenericRNP/planet_rotation.o)
   (environment/RNP/GenericRNP/RNP_messages.o)
   (utils/message/message_handler.o))

 

*******************************************************************************/

// System includes
#include <cstddef>
#include <cmath>

// JEOD includes
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/rotation_mars.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor, initialize low level data
 */
RotationMars::RotationMars (
   void)
:
   planet_rotational_velocity(0.0),
   nutation(NULL),
   use_full_rnp(true),
   phi_at_j2000(0.0),
   phi_spin(0.0)
{
   // Nothing else to do
}

/**
 * Destructor
 */
RotationMars::~RotationMars (
   void)
{
   // Nothing to do
}

/**
 * RotationMars specific implementation of update_rotation, for
 * axial rotation.
 */
void
RotationMars::update_rotation (
   void)
{

   // If full RNP not requested, just take the current time and multiply it
   // by the rotational velocity to get the updated Mars spin angle
   if (!use_full_rnp) {

      // Phi(t) = Phi_o + Phi_dot*time
      phi_spin = 0.0;
      phi_spin = current_time * planet_rotational_velocity;
   }
   else {

      // Request nutation correction from nutation
      if (nutation ==  NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, RNPMessages::setup_error,
            "RotationMars is not currently pointing "
            "to a nutation object.");
         return;
      }
      double psi_nut = 0.0;
      psi_nut = nutation->nutation_in_longitude;

      // Set up the relevant rotation equation from pg. 41 of Konopliv 2006:
      // Phi(t) = Phi_o + Phi_dot*time - Psi_nut * cos(obliquity angle)
      phi_spin = phi_at_j2000 + planet_rotational_velocity * current_time
                              - psi_nut * cos(nutation->obliquity_angle);

      if (phi_spin < 0.0) {
         phi_spin += 2.0 * M_PI;
      }
   }

   // Take trigs of the rotation angle to build the rotation matrix
   double cos_phi  = cos (phi_spin);
   double sin_phi  = sin (phi_spin);

   // Now populate the rotation matrix, as follows:
   // celestial frame = rot_z(-Phi) * mars frame
   rotation[0][0] = cos_phi;
   rotation[0][1] = -sin_phi;
   rotation[0][2] = 0.0;

   rotation[1][0] = sin_phi;
   rotation[1][1] = cos_phi;
   rotation[1][2] = 0.0;

   rotation[2][0] = 0.0;
   rotation[2][1] = 0.0;
   rotation[2][2] = 1.0;

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
