/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup GravityTorque
 * @{
 *
 * @file models/interactions/gravity_torque/src/gravity_torque.cc
 * Gravity gradient torque model
 */

/************************** TRICK HEADER****************************************
PURPOSE:
   ()

REFERENCE:
   (((None)))

ASSUMPTIONS AND LIMITATIONS:
   ((None))

Library dependencies:
   ((gravity_torque.o)
    (gravity_torque_messages.o)
    (dynamics/dyn_body/dyn_body.o)
    (utils/message/message_handler.o))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/gravity_torque.hh"
#include "../include/gravity_torque_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a GravityTorque object.
 */
GravityTorque::GravityTorque (
   void)
{
   subject_body = NULL;
   Vector3::initialize (torque);
   active = true;
}


/**
 * Destruct a GravityTorque object.
 */
GravityTorque::~GravityTorque (
   void)
{
   // empty for now
}


/**
 * Initialize a GravityTorque object.
 * \param[in,out] subject DynBody object subject to the torque
 */
void
GravityTorque::initialize (
   DynBody& subject)
{
   subject_body = &subject;

   return;
}


/**
 * Perform GravityTorque updates.
 */
void
GravityTorque::update (
   void)
{

   if (active == false) {
      torque[0] = 0.0;
      torque[1] = 0.0;
      torque[2] = 0.0;
      return;
   }

   if (subject_body == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, GravityTorqueMessages::initialization_error,
         "GravityTorque object was not properly initialized.");
      return;
   }

   double g[3][3];
   double inertia[3][3];

   // inertia tensor (in body frame)
   Matrix3x3::copy (subject_body->mass.composite_properties.inertia, inertia);

   // g = transform gradient matrix from inertial to body
   Matrix3x3::transform_matrix (
      subject_body->composite_body.state.rot.T_parent_this,
      subject_body->grav_interaction.grav_grad, g);

   // torque in body frame
   torque[0] =  g[1][2] * (inertia[2][2] - inertia[1][1]) -
               g[0][2] * inertia[0][1] +
               g[0][1] * inertia[0][2] -
               inertia[1][2] * (g[2][2] - g[1][1]);

   torque[1] =  g[0][2] * (inertia[0][0] - inertia[2][2]) +
               g[1][2] * inertia[0][1] -
               g[0][1] * inertia[1][2] -
               inertia[0][2] * (g[0][0] - g[2][2]);

   torque[2] =  g[0][1] * (inertia[1][1] - inertia[0][0]) -
               g[1][2] * inertia[0][2] +
               g[0][2] * inertia[1][2] -
               inertia[0][1] * (g[1][1] - g[0][0]);

   // Transform to structure.
   Vector3::transform_transpose (
      subject_body->mass.composite_properties.T_parent_this, torque);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
