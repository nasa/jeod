/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body.cc
 * Define base methods for the DynBody class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body.cc)
   (dyn_body_attach.cc)
   (dyn_body_collect.cc)
   (dyn_body_detach.cc)
   (dyn_body_find_body_frame.cc)
   (dyn_body_integration.cc)
   (dyn_body_initialize_model.cc)
   (dyn_body_propagate_state.cc)
   (dyn_body_set_state.cc)
   (dyn_body_vehicle_point.cc)
   (aux_classes.cc)
   (force.cc)
   (torque.cc)
   (dynamics/dyn_manager/src/dynamics_integration_group.cc)
   (dynamics/mass/src/mass.cc)
   (environment/gravity/src/gravity_interaction.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/


// System includes
#include <cstddef>
#include <algorithm> //std::find

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_manager/include/dynamics_integration_group.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"



//! Namespace jeod
namespace jeod {

// Construct a DynBody.
DynBody::DynBody ()
:
   mass(*this),
   name(mass.name),
   integ_frame_name(nullptr),
   integ_frame(nullptr),
   translational_dynamics(false),
   rotational_dynamics(false),
   compute_point_derivative(false),
   three_dof(false),
   rotation_integration(GeneralizedSecondOrderODETechnique::LieGroup),
   autoupdate_vehicle_points(true),
   grav_interaction(),
   dyn_manager(mass.dyn_manager),
   time_manager(nullptr),
   dyn_parent(nullptr),
   dyn_children(),
   initialized_states(RefFrameItems::No_Items),
   position_source(nullptr),
   velocity_source(nullptr),
   attitude_source(nullptr),
   rate_source(nullptr),
   integrated_frame(nullptr),
   associated_integrable_objects(),
   trans_integrator(),
   rot_integrator()
{
   // Register the checkpointable items.
   JEOD_REGISTER_CLASS (DynBody);
   JEOD_REGISTER_CLASS (BodyRefFrame);
   JEOD_REGISTER_CHECKPOINTABLE (this, trans_integrator);
   JEOD_REGISTER_CHECKPOINTABLE (this, rot_integrator);

   // This class integrates the composite body frame.
   integrated_frame = &composite_body;

   // We want the inverse inertia tensor (MassBody member) calculated.
   mass.compute_inverse_inertia = true;

   // The state is empty.
   initialized_states.set (RefFrameItems::No_Items);

   // Identify the mass points associated with the body ref frames.
   core_body.mass_point      = &mass.core_properties;
   composite_body.mass_point = &mass.composite_properties;
   structure.mass_point      = &mass.structure_point;
}


// Destruct a DynBody.
DynBody::~DynBody ()
{
   // Detach this body from its parent.
   if (dyn_parent != nullptr) {
       detach (*dyn_parent);
   }

   // Detach each child DynBody from this body.
   while (! dyn_children.empty()) {
      DynBody* child = dyn_children.back();
      child->detach (*this);
   }
   // Detach each sub MassBody from this body.
   while (! mass_children.empty()) {
      MassBody* sub = mass_children.back();
      remove_mass_body(*sub);
   }

   // Remove the body reference frames from the ref frame tree.
   core_body.remove_from_parent ();
   composite_body.remove_from_parent ();
   structure.remove_from_parent ();
   if( dyn_manager )
   {
       dyn_manager->remove_ref_frame( core_body );
       dyn_manager->remove_ref_frame( composite_body );
       dyn_manager->remove_ref_frame( structure );
   }

   // Remove the registrations of the checkpointable objects.
   JEOD_DEREGISTER_CHECKPOINTABLE (this, trans_integrator);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, rot_integrator);

   // Delete the allocated vehicle points, in reverse order.
   while (! vehicle_points.empty()) {
      auto* point = vehicle_points.back();
      if( dyn_manager )
      {
          dyn_manager->remove_ref_frame( *point );
      }
      if (JEOD_IS_ALLOCATED (point)) {
         JEOD_DELETE_OBJECT (point);
      }
      vehicle_points.pop_back();
   }
}

void
DynBody::set_name ( const std::string & name_in )
{
    mass.set_name(name_in);
}


// Indicates whether the current DynBody is a root body.
bool
DynBody::is_root_body ()
{
   return (dyn_parent == nullptr);
}


// Returns the parent of current DynBody's tree.
const DynBody *
DynBody::get_parent_body ()
const
{
   return dyn_parent;
}


// Returns the parent of current DynBody's tree.
DynBody *
DynBody::get_parent_body_internal ()
{
   return dyn_parent;
}


// Finds & returns root of current DynBody's tree.
const DynBody *
DynBody::get_root_body ()
const
{
   // Get the root body via the non-const version of this method.
   return
      (const_cast <DynBody *> (this))->DynBody::get_root_body_internal ();
}


// Finds & returns root of current DynBody's tree.
DynBody *
DynBody::get_root_body_internal ()
{

   DynBody * root_body;

   // Traverse the dyn_parent pointers until the top level DynBody is found
   root_body = this;
   while (root_body->dyn_parent != nullptr) {
      root_body = root_body->dyn_parent;
   }

   return root_body;
}


// Add a new GravityControls to the list in grav_interaction.
void
DynBody::add_control (
   GravityControls * control)
{
   // Pass the function call down to grav_interaction
   grav_interaction.add_control (control);
}


// Initialize the gravity controls of this DynBody.
void
DynBody::initialize_controls (
   GravityManager & grav_manager)
{
   // Pass the function call down to grav_interaction
   grav_interaction.initialize_controls(*dyn_manager, grav_manager);
}


// Make the frame subscriptions for each control consistent with the
// requirements for that control.
void
DynBody::reset_controls ()
{
   // Pass the function call down to grav_interaction
   grav_interaction.reset_controls(*dyn_manager);
}


// Sort the controls in increasing acceleration magnitude order.
void
DynBody::sort_controls ()
{
   // Pass the function call down to grav_interaction
   grav_interaction.sort_controls();
}


// Get a pointer to the DynamicsIntegrationGroup that integrates this DynBody.
DynamicsIntegrationGroup *
DynBody::get_dynamics_integration_group ()
{
   er7_utils::BaseIntegrationGroup * temp = get_integration_group();

   if (temp == nullptr) {
      return nullptr;
   }

   DynamicsIntegrationGroup *dig =
      dynamic_cast<DynamicsIntegrationGroup *>(temp);

   if (dig == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::internal_error,
         "This DynBody is being integrated by something "
         "other than a DynamicsIntegrationGroup\n"
         "The integration group must be compatible with "
         "a DynamicsIntegrationGroup");

      // Unreached
      return nullptr;
   }

   return dig;
}


// Associate an IntegrableObject to be integrated with this DynBody
void
DynBody::add_integrable_object (
   er7_utils::IntegrableObject & new_object)
{
   if (std::find (associated_integrable_objects.begin(),
                  associated_integrable_objects.end(),
                  &new_object) !=
       associated_integrable_objects.end()) {
      MessageHandler::warn (
         __FILE__, __LINE__,"Integrable Objects Addition:\n",
         "Integrable Object requested to be added to the list of \n"
         "associated integrable objects is already in the list.\n"
         "Addition failed.\n");
      return;
   }
   associated_integrable_objects.push_back(&new_object);
}

// Remove an IntegrableObject from this DynBody's list
void
DynBody::remove_integrable_object(
  er7_utils::IntegrableObject & del_object)
{
   std::vector< er7_utils::IntegrableObject*>::iterator iter =
      std::find (associated_integrable_objects.begin(),
                 associated_integrable_objects.end(),
                 &del_object);

   if (iter == associated_integrable_objects.end()) {
     MessageHandler::warn (
      __FILE__, __LINE__,"Integrable Objects removal:\n",
      "Integrable Object requested to be removed from the list of\n"
      "associated integrable objects was not found in the list.\n"
      "Removal failed.\n");
      return;
   }
   associated_integrable_objects.erase (iter);
}

// Remove all integrable objects associated with this DynBody object.
void
DynBody::clear_integrable_objects ()
{
   associated_integrable_objects.clear();
}

// Prepare all associated integrable objects associated
// with this dyn body to migrate to a new integration group
void DynBody::migrate_integrable_objects(void)
{
   JeodIntegrationGroup *group =
                             static_cast<JeodIntegrationGroup *>(
                             get_integration_group());

   if (group == nullptr) {
      MessageHandler::error (
         __FILE__, __LINE__, DynBodyMessages::invalid_group,
      "Integration group for DynBody %s is NULL.\n",
      name.c_str());
      return;
   }
   for (std::vector<er7_utils::IntegrableObject *>::const_iterator it =
           associated_integrable_objects.begin();
        it != associated_integrable_objects.end();
        ++it) {
      group->remove_integrable_object(**it);
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
