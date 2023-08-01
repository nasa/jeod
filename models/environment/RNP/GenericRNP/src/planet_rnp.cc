/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/src/planet_rnp.cc
 * Implement PlanetRNP
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((David A. Vallado)
    (Fundamentals of Astrodynamics and Applications, Third Edition)
    (Microcosm Press and Springer)
    (2007)))

Assumptions and limitations:
  ((This framework is limited to models which are of the form:
    celestial frame = Precession * Nutation *
                      Rotation * Polar Motion * terestrial frame
    Any individual component can be left out if needed, but this is the
    order present components will be multipled
    When the entire model is updated, it will be updated:
    Precession - Nutation - Rotation - Polar Motion
    This is important when individual models are inter-related
    This can be overriden by an inheriting class, if so needed
    ))

Library dependencies:
  ((planet_rnp.cc)
   (RNP_messages.cc)
   (dynamics/dyn_manager/src/dyn_manager.cc)
   (environment/planet/src/planet.cc)
   (utils/ref_frames/src/ref_frame_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/quaternion/src/quat_from_mat.cc))

 

*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/planet_rnp.hh"
#include "../include/planet_rotation.hh"
#include "../include/RNP_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor; constructs a PlanetRNP object.
 */
PlanetRNP::PlanetRNP (
   void)
:
   nutation(nullptr),
   precession(nullptr),
   polar_motion(nullptr),
   rotation(nullptr),
   rnp_type(FullRNP),
   enable_polar(true)
{
   Matrix3x3::initialize (NP_matrix);
}

/**
 * Class destructor
 */
PlanetRNP::~PlanetRNP (
   void)
{//empty
}



/**
 * Invokes the calculation for all rotation models contained in the
 * RNP, used on the last time set in each model through
 * PlanetRotation::set_time. Then multiplies out and updates the
 * attitude of the planet found during initialization.
 */
void
PlanetRNP::update_rnp (
   void)
{


   // Update the nutation and precession, checking both that they are
   // required and that they exist
   if (rnp_type == FullRNP) {
      if (nutation == nullptr) {
         MessageHandler::fail (
            __FILE__, __LINE__, RNPMessages::fidelity_error,
            "The PlanetRotation* nutation pointer"
            " was not set in the PlanetRNP object, which is"
            " required with a full fidelity RNP\n");
      }
      else {
         nutation->update_rotation();
      }
      if (precession == nullptr) {
         MessageHandler::fail (
            __FILE__, __LINE__, RNPMessages::fidelity_error,
            "The PlanetRotation* precession pointer"
            " was not set in the PlanetRNP object, which is"
            " required with a full fidelity RNP\n");
      }
      else {
         precession->update_rotation();
         // If it is a full fidelity RNP, then we need to update the
         // internal NP matrix
         Matrix3x3::product_transpose_transpose (
            nutation->rotation,
            precession->rotation,
            NP_matrix);
      }
   }

   // Update the rotation, first checking if it has been set to something
   if (rotation == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::setup_error,
         "The PlanetRotation* rotation pointer was"
         " not set in the PlanetRNP object, causing an error\n");
   }
   else {
      rotation->update_rotation();
   }

   // Update the polar motion, checking if it has been set to something
   // Only do this if polar motion is enabled
   if (enable_polar == true) {
      if (polar_motion == nullptr) {
         MessageHandler::fail (
            __FILE__, __LINE__, RNPMessages::fidelity_error,
            "The PlanetRotation* polar_motion pointer"
            " was not set in the PlanetRNP object, which is"
            " required with polar motion enabled\n");
      }
      else {
         polar_motion->update_rotation();
      }
   }

   propagate_rnp();

   return;
}

/**
 * Same as update_rnp, but only the axial_rotation will be updated
 */
void
PlanetRNP::update_axial_rotation (
   void)
{


   // Update the rotation, first checking if it has been set to something
   if (rotation == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::setup_error,
         "The PlanetRotation* rotation pointer was"
         " not set in the PlanetRNP object, causing an error\n");
   }
   else {
      rotation->update_rotation();
   }

   propagate_rnp();

   return;
}

/**
 * Multiples out the (up to) four planet rotation models (nutation,
 * precession, polar motion and rotation), leaving out models whose
 * pointers are NULL, and feeds it to the planet attitude found
 * in the dyn manager given in initialize
 */
void
PlanetRNP::propagate_rnp (
   void)
{

   // rotation should ALWAYS going to be present, so check if it is present

   if (rotation == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::setup_error,
         "The PlanetRotation* rotation pointer was"
         " not set in the PlanetRNP object, causing an error\n");
   }

// order is:
// polar_motion^T * rotation^T * nutation^T precession^T

   // If rnp_type == Identity_RNP, only rotation exists, so just copy
   // that into the final number and return the function
   if (rnp_type == RotationOnly) {
      Matrix3x3::transpose (
         rotation->rotation, planet_rot_state->T_parent_this);
      // set the rotational velocity components, and calculate the quaternion
      // from the already set transformation matrix
      planet_rot_state->ang_vel_mag     = planet_omega;
      planet_rot_state->ang_vel_this[2] = planet_omega;
      planet_rot_state->compute_quaternion();
      return;
   }
   // If it's a full fidelity RNP, calculate NP


   // Rotation is ALWAYS involved. If polar motion is involved, copy
   // polar_motion^T * rotation^T into the temp matrix. Otherwise,
   // copy only rotation^T into the temp matrix.
   if (enable_polar == true) {
      if (polar_motion == nullptr) {
         MessageHandler::fail (
            __FILE__, __LINE__, RNPMessages::fidelity_error,
            "The PlanetRotation* polar_motion pointer"
            " was not set in the PlanetRNP object, which is"
            " required with polar motion enabled\n");
      }
      else {
         Matrix3x3::product_transpose_transpose (
            polar_motion->rotation, rotation->rotation, scratch_matrix);
      }
   }
   else {
      Matrix3x3::transpose (rotation->rotation, scratch_matrix);
   }

   // If rnp_type is NOT Identity_RNP, multiply rotation^T * NP
   // scratch_matrix currently contains either
   // polar_motion^T * rotation^T, or rotation^T, depending on if
   // polar motion is enabled.
   // Note that if NP needs to be calculated, it was already calculated
   // in "update_rnp"
   if ((rnp_type == FullRNP) || (rnp_type == ConstantNP)) {
      Matrix3x3::product (
         scratch_matrix, NP_matrix, planet_rot_state->T_parent_this);
   }


   // set the rotational velocity components, and calculate the quaternion
   // from the already set transformation matrix
   planet_rot_state->ang_vel_mag     = planet_omega;
   planet_rot_state->ang_vel_this[2] = planet_omega;
   planet_rot_state->compute_quaternion();

   return;

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
