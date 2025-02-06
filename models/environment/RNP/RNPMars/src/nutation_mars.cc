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
 * @file models/environment/RNP/RNPMars/src/nutation_mars.cc
 * Implementation for the NutationMars class
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
((nutation_mars.o)
(environment/RNP/GenericRNP/RNP_messages.o)
(environment/RNP/GenericRNP/planet_rotation.o)
(utils/sim_interface/memory_interface.o)
(utils/message/message_handler.o))

 

*******************************************************************************/

// System includes
#include <cstddef>
#include <cmath>

// JEOD includes
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/nutation_mars.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor
 */
NutationMars::NutationMars (
   void)
:
   nutation_in_longitude(0.0),
   nutation_in_obliquity(0.0),
   I_at_j2000(0.0),
   I_dot(0.0),
   obliquity_angle(0.0),
   mean_motion(0.0),
   mean_anomaly_j2000(0.0),
   q_angle_j2000(0.0),
   I_m_orig(NULL),
   psi_m_orig(NULL),
   int_to_double(NULL)
{
}


/**
 * Destructor
 */
NutationMars::~NutationMars (
   void)
{
   if (I_m_orig != NULL) {
      JEOD_DELETE_ARRAY (I_m_orig);
      I_m_orig = NULL;
   }
   if (psi_m_orig != NULL) {
      JEOD_DELETE_ARRAY (psi_m_orig);
      psi_m_orig = NULL;
   }
   if (int_to_double != NULL) {
      JEOD_DELETE_ARRAY (int_to_double);
      int_to_double = NULL;
   }

   return;
}


/**
 * NutationMars specific implementation of update_rotation, used here
 * to describe nutation effects in longitude and obliquity
 */
void
NutationMars::update_rotation (
   void)
{

   // Cache off time locally for speedup and legibility
   double time  = current_time;

   // Set up the equations to calculate the nutation corrections, from pg. 40
   // of Konopliv 2006:
   // Psi_nut = sum[m=1..9]{Psi_m * sin(alpha_m*time + theta_m)}
   // I_nut = I_oo + sum[m=1..9]{I_m * cos(alpha_m*time + theta_m)}

   // Set up nutation parameters for zeroth step of summation
   nutation_in_longitude = 0.0;
   nutation_in_obliquity = I_m_orig[0];

   // Initialize the variables used in the summation
   double alpha_m = 0.0;
   double theta_m = 0.0;

   // Perform the summation stackup
   for (unsigned int ii = 1; ii <= 9; ++ii) {

      if (ii <= 3) {
         alpha_m = int_to_double[ii] * mean_motion;
         theta_m = int_to_double[ii] * mean_anomaly_j2000;
      }
      else {
         alpha_m = (int_to_double[ii] - 3.0) * mean_motion;
         theta_m = (int_to_double[ii] - 3.0) * mean_anomaly_j2000 +
                   q_angle_j2000;
      }

      nutation_in_longitude += psi_m_orig[ii] * sin(alpha_m * time + theta_m);
      nutation_in_obliquity += I_m_orig[ii] * cos(alpha_m * time + theta_m);
   }


   // Next, calculate the current obliquity angle, from pg 39 of Konopliv 2006:
   // I(t) = I_o + I_dot*time + I_nut
   obliquity_angle = I_at_j2000 + I_dot * time + nutation_in_obliquity;

   // Take trigs of the obliquity angle to build its rotation matrix
   double cos_I = cos (obliquity_angle);
   double sin_I = sin (obliquity_angle);

   // Populate the rotation matrix as follows:
   // celestial frame = rot_x(-I) * mars frame
   Matrix3x3::initialize (rotation);

   rotation[0][0] = 1.0;
   rotation[0][1] = 0.0;
   rotation[0][2] = 0.0;

   rotation[1][0] = 0.0;
   rotation[1][1] = cos_I;
   rotation[1][2] = -sin_I;

   rotation[2][0] = 0.0;
   rotation[2][1] = sin_I;
   rotation[2][2] = cos_I;

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
