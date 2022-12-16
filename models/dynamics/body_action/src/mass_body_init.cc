/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/mass_body_init.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((mass_body_init.o)
   (body_action.o)
   (body_action_messages.o)
   (dynamics/mass/mass.o)
   (dynamics/mass/mass_point_state.o)
   (dynamics/mass/mass_properties_init.o)
   (environment/ephemerides/ephem_interface/ephem_ref_frame.o)
   (utils/message/message_handler.o)
   (utils/ref_frames/ref_frame.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/mass_body_init.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a MassBodyInit.
 */
MassBodyInit::MassBodyInit (
   void)
:
   properties(),
   points(NULL),
   num_points(0)
{
   return;
}


/**
 * Initialize the core mass properties of the subject MassBody.
 * \param[in,out] dyn_manager Jeod manager
 */
void
MassBodyInit::apply (
   DynManager & dyn_manager)
{

   // Initialize the mass properties and set the mass points.
   subject->initialize_mass (properties, points, num_points);

   // Debug.
   MessageHandler::debug (
      __FILE__, __LINE__, BodyActionMessages::trace,
      "%s: %s mass properties initialized.",
      action_identifier, subject->name.c_str());


   // Forward the action up the chain.
   BodyAction::apply (dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
