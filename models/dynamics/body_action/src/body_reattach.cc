/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_reattach.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_reattach.cc)
   (body_action.cc)
   (body_action_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (dynamics/mass/src/mass_reattach.cc)
   (utils/message/src/message_handler.cc)
   (utils/orientation/src/orientation.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/body_reattach.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Construct a MassBodyReattach.
 */
BodyReattach::BodyReattach()
{
    active = false;
}

/**
 * Initialize the core mass properties of the subject MassBody.
 * \param[in,out] dyn_manager Jeod manager
 */
void BodyReattach::apply(DynManager & dyn_manager)
{
    // Convert user-specified orientation to transformation matrix.
    pstr_cstr.compute_transform();

    // Rettach the body.
    bool succeeded = mass_subject->reattach(offset_pstr_cstr_pstr, pstr_cstr.trans);

    // Terminate the sim if requested to do so on failed attachments.
    if(!succeeded)
    {
        if(terminate_on_error)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 BodyActionMessages::fatal_error,
                                 "%s failed:\n"
                                 "terminate_on_failure flag set and attachment error occurred.\n"
                                 "The attachment error described above is fatal per this setting.",
                                 action_identifier.c_str());
        }
        else
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  BodyActionMessages::invalid_object,
                                  "%s: %s failed to reattach.",
                                  action_identifier.c_str(),
                                  mass_subject->name.c_str());
        }
    }

    // Debug.
    if(succeeded)
    {
        MessageHandler::debug(__FILE__,
                              __LINE__,
                              BodyActionMessages::trace,
                              "%s: %s reattached.",
                              action_identifier.c_str(),
                              mass_subject->name.c_str());
    }

    // Forward the action up the chain.
    BodyAction::apply(dyn_manager);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
