/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_attach_aligned.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_attach_aligned.o)
   (body_action.o)
   (body_action_messages.o)
   (body_attach.o)
   (dynamics/mass/mass_point_state.o)
   (dynamics/dyn_manager/dyn_manager.o)
   (dynamics/dyn_body/dyn_body_attach.o)
   (dynamics/mass/mass_attach.o)
   (utils/message/message_handler.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/body_attach_aligned.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a MassBodyAttachAligned.
 */
BodyAttachAligned::BodyAttachAligned (
   void)
:
   subject_point_name(),
   parent_point_name()
{
  ; // Empty
}


/**
 * Initialize a MassBodyAttach.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
BodyAttachAligned::initialize (
   DynManager & dyn_manager)
{

   // Forward the request up the chain.
   BodyAttach::initialize (dyn_manager);

   // Sanity check: Names must be supplied.
   if (subject_point_name.empty()) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "Empty subject_point_name string",
         action_identifier.c_str());
      return;
   }

   if (parent_point_name.empty()) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "Empty parent_point_name string",
         action_identifier.c_str());
      return;
   }

   return;
}


/**
 * Initialize the core mass properties of the subject MassBody.
 * \param[in,out] dyn_manager Jeod manager
 */
void
BodyAttachAligned::apply (
   DynManager & dyn_manager)
{
    // DynBody to DynBody
    if( dyn_subject != nullptr && dyn_parent != nullptr )
    {
        succeeded = dyn_parent->attach_child( parent_point_name.c_str(),
                                        subject_point_name.c_str(),
                                        *dyn_subject);
    }
    // MassBody subbody/subassembly to DynBody
    else if( dyn_subject == nullptr && dyn_parent != nullptr )
    {
        succeeded = dyn_parent->add_mass_body( parent_point_name.c_str(),
                                               subject_point_name.c_str(),
                                               *mass_subject);
    }
    // MassBody/MassBody assembly to MassBody
    else if( dyn_subject == nullptr && dyn_parent == nullptr )
    {
        succeeded = mass_subject->attach_to ( subject_point_name.c_str(),
                                         parent_point_name.c_str(),
                                         *mass_parent);
    }
    // DynBody to MassBody -- ILLEGAL
    else // if( dyn_subject != nullptr && dyn_parent == nullptr )
    {
        succeeded = false;
    }

    // Forward the action up the chain. The parent class deals with status.
    BodyAttach::apply (dyn_manager);

    return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
