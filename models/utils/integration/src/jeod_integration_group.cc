/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/src/jeod_integration_group.cc
 * Define JeodIntegrationGroup methods.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((jeod_integration_group.o)
   (jeod_integration_time.o)
   (integration_messages.o)
   (utils/message/message_handler.o)
  )

 
******************************************************************************/


// Local includes
#include "../include/jeod_integration_group.hh"

#include "../include/jeod_integration_time.hh"
#include "../include/integration_messages.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrable_object.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"

// System includes
#include <algorithm>
#include <cstddef>


//! Namespace jeod
namespace jeod {

// JeodIntegrationGroup class registry.
void
JeodIntegrationGroup::register_classes ()
{
   JEOD_REGISTER_CLASS (er7_utils::BaseIntegrationGroup);
   JEOD_REGISTER_CLASS (JeodIntegrationGroup);
   JEOD_REGISTER_CLASS (JeodIntegrationGroupOwner);
   JEOD_REGISTER_CLASS (er7_utils::IntegrableObject);
}


// JeodIntegrationGroup default constructor, needed for checkpoint/restart.
JeodIntegrationGroup::JeodIntegrationGroup ()
:
   BaseIntegrationGroup(),
   group_owner(NULL),
   integ_merger(),
   jeod_integ_interface(NULL),
   jeod_time_manager(NULL),
   integrable_objects()
{
   register_classes ();

   JEOD_REGISTER_CHECKPOINTABLE (this, integrable_objects);
}


// JeodIntegrationGroup non-default constructor.
JeodIntegrationGroup::JeodIntegrationGroup (
   JeodIntegrationGroupOwner & owner,
   er7_utils::IntegratorConstructor & integ_cotr,
   JeodIntegratorInterface & integ_inter,
   JeodIntegrationTime & time_mngr)
:
   BaseIntegrationGroup(integ_cotr, integ_inter, time_mngr),
   group_owner(&owner),
   integ_merger(),
   jeod_integ_interface(&integ_inter),
   jeod_time_manager(&time_mngr),
   integrable_objects()
{
   register_classes ();

   JEOD_REGISTER_CHECKPOINTABLE (this, integrable_objects);

   integ_merger.configure (integ_cotr);
   time_mngr.add_time_change_subscriber (*this);
}


// JeodIntegrationGroup destructor.
JeodIntegrationGroup::~JeodIntegrationGroup ()
{
   JEOD_DEREGISTER_CHECKPOINTABLE (this, integrable_objects);

   if (jeod_time_manager != NULL) {
      jeod_time_manager->remove_time_change_subscriber (*this);
   }
}


// Add an integrable object to the vector of such.
void
JeodIntegrationGroup::add_integrable_object (
   er7_utils::IntegrableObject & integrable_object)
{
   // Remove the entry for this object if it's already present.
   std::vector<er7_utils::IntegrableObject*>::iterator iter =
      std::find (integrable_objects.begin(), integrable_objects.end(),
                 &integrable_object);
   if (iter != integrable_objects.end()) {
      // Make the object destroy its integrator(s).
      if (integ_controls != NULL) {
         integrable_object.destroy_integrators ();
      }

       // Delete the object from the list of integrable objects.
      integrable_objects.erase (iter);
   }

   // Make the object create its integrator(s).
   if (integ_controls != NULL) {
      integrable_object.set_integration_group (*this);
      integrable_object.create_integrators (
         *integ_constructor, *integ_controls, *time_interface);
   }

   // Add the object to the list of integrable objects.
   integrable_objects.push_back (&integrable_object);
}


// Remove an integrable object from the vector of such.
void
JeodIntegrationGroup::remove_integrable_object (
   er7_utils::IntegrableObject & integrable_object)
{
   // Find the entry for this object in the vector of such.
   std::vector<er7_utils::IntegrableObject*>::iterator iter =
      std::find (integrable_objects.begin(), integrable_objects.end(),
                 &integrable_object);

   // Not found: Nothing to delete.
   if (iter == integrable_objects.end()) {
      MessageHandler::error (
         __FILE__, __LINE__,
         IntegrationMessages::invalid_item,
         "Missing entry in IntegrationGroup::remove_integrable_object()");
      return;
   }

   // Make the object destroy its integrator(s).
   if (integ_controls != NULL) {
      integrable_object.destroy_integrators ();
   }

    // Delete the object from the list of integrable objects.
   integrable_objects.erase (iter);
}


/**
 * Initialize the integration group.
 */
void
JeodIntegrationGroup::initialize_group ()
{
   // Initialize as a base integration group.
   BaseIntegrationGroup::initialize_group();

   // Tell each registered integrable object to create state integrators.
   for (std::vector<er7_utils::IntegrableObject*>::iterator iter =
           integrable_objects.begin();
        iter != integrable_objects.end();
        ++iter) {
      er7_utils::IntegrableObject * integrable_object = *iter;
      integrable_object->set_integration_group (*this);
      integrable_object->create_integrators (
         *integ_constructor, *integ_controls, *time_interface);
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
