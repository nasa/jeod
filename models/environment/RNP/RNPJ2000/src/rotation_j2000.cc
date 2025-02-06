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
 * @file models/environment/RNP/RNPJ2000/src/rotation_j2000.cc
 * Implementation of RotationJ2000
 */

/*******************************************************************************

Purpose:
  ()

Reference:
      (((Mulcihy, David D. and Bond, Victor R.)
       (The RNP Routine for the STandard Epoch J2000)
       (NASA:JSC-24574) (NASA Engineering Directorate, Navigation, Control,
        and Aeronautics Division) (September 1990) (--)))

Assumptions and limitations:
  ((Earth specific))

Class:
  (RotationJ2000)

Library dependencies:
  ((rotation_j2000.o)
   (environment/RNP/GenericRNP/RNP_messages.o)
   (environment/RNP/GenericRNP/planet_rotation.o)
   (utils/message/message_handler.o))

 

*******************************************************************************/

// System includes
#include <cstddef>
#include <cmath>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"

// Model includes
#include "../include/rotation_j2000.hh"


//! Namespace jeod
namespace jeod {

/**
 * default constructor, initialize low level data
 */
RotationJ2000::RotationJ2000 (
   void)
:
   planet_rotational_velocity(0.0),
   nutation(NULL),
   use_full_rnp(true)
{
// empty for now
}

/**
 * destructor
 */
RotationJ2000::~RotationJ2000 (
   void)
{
// empty
}

/**
 * J2000 specific implementation of update_rotation, from
 * PlanetRotation. For axial rotation
 */
void
RotationJ2000::update_rotation (
   void)
{
   // if not use_full_rnp, just take the current set time and
   // multiply it by the rotational velocity to get the theta_gast
   // angle
   if (!use_full_rnp) {
      theta_gast = 0.0;
      theta_gast = current_time * planet_rotational_velocity;
   }

   else {

      // Unlike JEOD 1.5, we now start out with a fully calculated GMST
      // time. So just start from there and enjoy.
      double theta_gmst = current_time;

      // ask nutation for equation of the equinoxes

      if (nutation ==  NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, RNPMessages::setup_error,
            "RotationJ2000 is not currently pointing"
            " to a nutation object");
      }

      // The following code is a direct port from Jeod 1.52 . According to Vallado,
      // the divide by 240.0 is converting from sidereal seconds to degrees
      // turned (86400 sidereal seconds per 360 degrees)

      theta_gast =
         ((theta_gmst + nutation->equa_of_equi) / 240.0) * DEGTORAD;
      double temp = (theta_gast / (2.0 * M_PI));
      theta_gast = (temp - static_cast<int> (temp)) * 2.0 * M_PI;

      if (theta_gast < 0.0) {
         theta_gast += 2.0 * M_PI;
      }
   }

   // create the angles for the transformation matrix
   double cos_ang = 0.0;
   double sin_ang = 0.0;
   cos_ang = cos (theta_gast);
   sin_ang = sin (theta_gast);

   // populate the transformation matrix
   rotation[0][0] = cos_ang;
   rotation[0][1] = -sin_ang;
   rotation[0][2] = 0.0;

   rotation[1][0] = sin_ang;
   rotation[1][1] = cos_ang;
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
