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
 * @file models/environment/RNP/RNPMars/src/precession_mars.cc
 * Implementation of PrecessionMars
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
((precession_mars.cc)
(environment/RNP/GenericRNP/src/RNP_messages.cc)
(environment/RNP/GenericRNP/src/planet_rotation.cc)
(utils/message/src/message_handler.cc))

 

*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/precession_mars.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor
 */
PrecessionMars::PrecessionMars (
      void)
:
   nutation(nullptr),
   psi_at_j2000(0.0),
   psi_dot(0.0),
   psi_precess(0.0),
   N(0.0),
   J(0.0)
{
   Matrix3x3::initialize(NJ_matrix);
}


/**
 * Destructor
 */
PrecessionMars::~PrecessionMars (
      void)
{
   // Nothing to do
}


/**
 * PrecessionMars specific implementation of update_rotation,
 * to calculate precession.
 */
void
PrecessionMars::update_rotation (
      void)
{

   // Set up the relevant precession equation from pg. 39 of Konopliv 2006:
   // Psi(t) = Psi_o + Psi_dot*time + Psi_nut

   // Access nutation of longitude term for use in equation
   if (nutation ==  nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::setup_error,
         "PrecessionMars is not currently pointing "
         "to a nutation object.");
      return;
   }
   double psi_nut = 0.0;
   psi_nut = nutation->nutation_in_longitude;

   // Calculate the current precession angle
   psi_precess = psi_at_j2000 + psi_dot * current_time + psi_nut;

   // Take trigs of the precession angle to build the precession matrix
   double cos_psi  = cos (psi_precess);
   double sin_psi  = sin (psi_precess);

   double precession_matrix[3][3];
   // Now populate the precession matrix, as follows:

   precession_matrix[0][0] = cos_psi;
   precession_matrix[0][1] = -sin_psi;
   precession_matrix[0][2] = 0.0;

   precession_matrix[1][0] = sin_psi;
   precession_matrix[1][1] = cos_psi;
   precession_matrix[1][2] = 0.0;

   precession_matrix[2][0] = 0.0;
   precession_matrix[2][1] = 0.0;
   precession_matrix[2][2] = 1.0;

   // celestial frame = NJ_matrix * rot_z(-Psi) * mars frame
   Matrix3x3::product(NJ_matrix, precession_matrix, rotation);

   return;
}

/**
 * Calculate constant rotation matrices resulting from N and J.
 */
void
PrecessionMars::compute_fixed_matrices (
      void)
{

   // Take trigs of the angles to build their rotation matrices
   double cos_N = cos (N);
   double sin_N = sin (N);
   double cos_J = cos (J);
   double sin_J = sin (J);

   // Populate the N rotation matrix as follows:
   // celestial frame = rot_z(-N) * mars frame
   double N_matrix[3][3];
   Matrix3x3::initialize (N_matrix);
   N_matrix[0][0] = cos_N;
   N_matrix[0][1] = -sin_N;
   N_matrix[0][2] = 0.0;

   N_matrix[1][0] = sin_N;
   N_matrix[1][1] = cos_N;
   N_matrix[1][2] = 0.0;

   N_matrix[2][0] = 0.0;
   N_matrix[2][1] = 0.0;
   N_matrix[2][2] = 1.0;


   // Populate the J rotation matrix as follows:
   // celestial frame = rot_x(-J) * mars frame
   double J_matrix[3][3];
   Matrix3x3::initialize (J_matrix);
   J_matrix[0][0] = 1.0;
   J_matrix[0][1] = 0.0;
   J_matrix[0][2] = 0.0;

   J_matrix[1][0] = 0.0;
   J_matrix[1][1] = cos_J;
   J_matrix[1][2] = -sin_J;

   J_matrix[2][0] = 0.0;
   J_matrix[2][1] = sin_J;
   J_matrix[2][2] = cos_J;


   // Combine the matrices and store in the appropriate member data variable
   Matrix3x3::product(N_matrix, J_matrix, NJ_matrix);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
