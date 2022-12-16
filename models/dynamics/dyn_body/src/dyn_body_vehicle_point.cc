/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_vehicle_point.cc
 * Define methods that support vehicle points.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_vehicle_point.o)
   (dyn_body_messages.o)
   (dynamics/mass/mass.o)
   (utils/ref_frames/ref_frame.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/quaternion/include/quat.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"


//! Namespace jeod
namespace jeod {

// Add a mass point to the dyn body's list of such and
// make a vehicle point that corresponds to the added mass point.
void
DynBody::add_mass_point (
   const MassPointInit & mass_point_init)
{
   MassPoint * mass_point;
   BodyRefFrame * point_frame;

   // Sanity check: The subject must be registered with the dynamics manager.
   if ((dyn_manager == nullptr) ||
       (! dyn_manager->is_dyn_body_registered (this))) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::invalid_body,
         "DynBody '%s' has not been initialized.\n",
         name.c_str());
   }

   // Add the mass point as a mass point.
   mass.add_mass_point (mass_point_init);

   // The newly-constructed will be the last point in the list.
   mass_point = mass.mass_points.back ();

   // Allocate the vehicle point (a reference frame).
   point_frame = JEOD_ALLOC_CLASS_OBJECT (BodyRefFrame, ());

   // Give the vehicle point the same name as the mass point's name.
   point_frame->set_name (mass_point->get_name());

   // Associate the vehicle point with the mass point.
   point_frame->mass_point = static_cast <MassPoint *> (mass_point);

   // Associate the vehicle point with the vehicle.
   point_frame->set_owner (this);

   // Attach the vehicle point frame to the integration frame.
   if (integ_frame != nullptr) {
      integ_frame->add_child (*point_frame);
   }

   // Add the vehicle point to the body's list of such.
   vehicle_points.push_back (point_frame);

   // Register the frame with the dynamics manager.
   dyn_manager->add_ref_frame (*point_frame);
}


// Find the vehicle point with the given name.
const BodyRefFrame *
DynBody::find_vehicle_point (
   const char * pt_name)
const
{
   std::size_t search_offset = name.size() + 1;
   const BodyRefFrame * found_point = nullptr;

   // Only search if the name is valid. (No else; found point is already NULL)
   if ((pt_name != nullptr) && (*pt_name != '\0')) {
      const char * pt_suffix = name.suffix(pt_name);
      // Search for the point.
      for (const auto* point : vehicle_points) {
         if (std::strcmp (pt_suffix, point->get_name()+search_offset) == 0) {
            found_point = point;
            break;
         }
      }
   }

   return found_point;
}

/**
 * Compute the state derivatives at a vehicle point.
 *
 * @param[in] frame Vehicle point reference frame
 * @param[out] derivs Computed derivatives
 */
void
DynBody::compute_vehicle_point_derivatives (
   const BodyRefFrame & vehicle_pt,
   FrameDerivs & pt_derivs)
{
   // The root body for this body.
   // Only the root body's derivs element contains valid data.
   const DynBody * root_body = get_root_body();

   // The root body's composite mass properties.
   const MassProperties & root_ppty = root_body->mass.composite_properties;

   // The root body's composite body rotational state.
   const RefFrameRot & root_rot = root_body->composite_body.state.rot;

   // The derivatives for this root body.
   const FrameDerivs & root_derivs = root_body->derivs;

   // Other local variables
   MassPointState cm_to_point;  // --   Vehicle pt state wrt CoM
   double cm_to_point_inrtl[3]; // M    Point wrt CoM, inertial coordinates
   double wxr[3];               // M/s  Omega cross r
   double wxwxr[3];             // M/s2 Centrifugal accel
   double axr[3];               // M/s2 Acceleration due to angular accel
   double accel_rot_body[3];    // M/s2 Acceleration due to rotation
   double accel_rot_inrtl[3];   // M/s2 Acceleration due to rotation
   double delta_acc_grav[3];    // M/s2 Acceleration due to gravity gradient
   double delta_acc_non_g[3];   // M/s2 Delta acceleration, non-gravity


   // Sanity check: Never update the root body's derivs structure. Ever.
   if (&pt_derivs == &root_derivs) {
      // Complain vehemently if the attempt is utterly invalid.
      // (Requesting the root body to update its composite derivatives
      // is silly, but no harm, no foul, so no complaint.)
      if (&vehicle_pt != &root_body->composite_body) {
            MessageHandler::error (
               __FILE__, __LINE__, DynBodyMessages::invalid_frame,
               "Illegal attempt to update the root body '%s' FrameDerivs\n",
               root_body->name.c_str());
      }
      return;
   }


   // Compute the vehicle point state wrt the composite center of mass.
   vehicle_pt.mass_point->compute_relative_state (root_ppty, cm_to_point);

   // Transform to position to inertial.
   Vector3::transform_transpose (root_rot.T_parent_this,
                                 cm_to_point.position,
                                 cm_to_point_inrtl);

   // Compute accelerations caused by vehicle rotation.
   Vector3::cross (root_rot.ang_vel_this, cm_to_point.position, wxr);
   Vector3::cross (root_rot.ang_vel_this, wxr, wxwxr);
   Vector3::cross (root_derivs.rot_accel, cm_to_point.position, axr);
   Vector3::sum (wxwxr, axr, accel_rot_body);
   Vector3::transform_transpose (root_rot.T_parent_this,
                                 accel_rot_body,
                                 accel_rot_inrtl);

   // Compute delta gravitational acceleration.
   Vector3::transform (grav_interaction.grav_grad, cm_to_point_inrtl,
                       delta_acc_grav);

   // Compute accelerations.
   Vector3::diff (accel_rot_inrtl, delta_acc_grav, delta_acc_non_g);
   Vector3::sum (root_derivs.non_grav_accel, delta_acc_non_g,
                 pt_derivs.non_grav_accel);
   Vector3::sum (root_derivs.trans_accel, accel_rot_inrtl,
                 pt_derivs.trans_accel);
   Vector3::transform (cm_to_point.T_parent_this, root_derivs.rot_accel,
                       pt_derivs.rot_accel);
   vehicle_pt.state.rot.Q_parent_this.compute_left_quat_deriv (
      vehicle_pt.state.rot.ang_vel_this, pt_derivs.Qdot_parent_this);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
