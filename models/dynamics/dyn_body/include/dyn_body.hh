//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/include/dyn_body.hh
 * Define the class DynBody.
 */

/*******************************************************************************

Purpose:
()

Library dependencies:
((dyn_body.o))


*******************************************************************************/


#ifndef JEOD_DYN_BODY_HH
#define JEOD_DYN_BODY_HH

// system includes
#include <vector>
#include <list>

// Model includes
#include "body_ref_frame.hh"
#include "body_force_collect.hh"
#include "frame_derivs.hh"

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "environment/gravity/include/gravity_interaction.hh"
#include "utils/container/include/simple_checkpointable.hh"
#include "utils/integration/include/generalized_second_order_ode_technique.hh"
#include "utils/integration/include/restartable_state_integrator.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/ref_frames/include/ref_frame_interface.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrable_object.hh"
#include "er7_utils/integration/core/include/integrator_result.hh"
#include "er7_utils/integration/core/include/integrator_result_merger_container.hh"

// System includes


//! Namespace jeod
namespace jeod {

class JeodIntegrationTime;
class DynamicsIntegrationGroup;
class BaseDynManager;
class EphemerisRefFrame;


/**
 * Class DynBody is the base class for all dynamic bodies.
 * A DynBody is a MassBody that is connected to the outside world.
 * These connections are in the form of three reference frames tied to the
 * body -- the structural, core body, and composite body frames.
 *
 * For a non-root body, the states for each of these frames is calculated
 * based on the parent body's state and on the body attachment.
 *
 * For a root body, one of these three frames must be integrated.
 * The details of how that integration is performed is the subject of classes
 * that derive from DynBody.
 */
class DynBody : virtual public RefFrameOwner,
   virtual public er7_utils::IntegrableObject {

   JEOD_MAKE_SIM_INTERFACES(DynBody)

public:

   // Member functions

   // Constructor and destructor
   // Note: The copy constructor and assignment operator are deleted.

   /**
    * DynBody default constructor.
    */
   DynBody ();

   /**
    * DynBody destructor.
    */
   virtual ~DynBody ();


   /**
    * Initialize internal and external interrelations, including registration
    / with the dynamics manager.
    * @param[in,out] dyn_manager_in Dynamics manager
    */
   virtual void initialize_model (BaseDynManager & dyn_manager_in);


   // Required methods from RefFrameOwner

   /**
    * Activate a DynBody object.
    * The current implementation does nothing.
    * DynBody objects are always active.
    */
   void activate ()
   {}

   /**
    * Deactivate a DynBody object.
    * The current implementation does nothing.
    * DynBody objects are always active.
    */
   void deactivate ()
   {}

   /**
    * Set the name of the vehicle.
    *
    * @param[in] name_in Name of this body
    */
   void set_name ( std::string name_in );


   // Gravity controls convenience methods (pass through to grav_interaction)

   /**
    * Add a new GravityControls to the list in grav_interaction.
    * @param[in] control Control to be added
    */
   virtual void add_control (GravityControls * control);

   /**
    * Initialize the gravity controls of this DynBody.
    *
    * @note Initialization phasing:@n
    * The following must have been called prior to calling this method:
    *   - GravityManager::initialize_model to register the GravityManager object
    *     with the dynamics manager.
    *   - GravityManager::add_grav_source to register the pertinent GravitySource
    *     objects with the Gravity Manager.
    *   - Planet::register_model to associate the planet with a GravitySource.
    *
    * @param[in] grav_manager Reference to Gravity Manager
    */
   virtual void initialize_controls (GravityManager & grav_manager);

   /**
    * Make the frame subscriptions for each control consistent with the
    * requirements for that control.
    */
   virtual void reset_controls ();

   /**
    * Sort the gravity controls in ascending acceleration magnitude order.
    */
   virtual void sort_controls ();


   // Integration support methods

   /**
    * Collect forces and torques acting on the vehicle.
    */
   virtual void collect_forces_and_torques ();

   /**
    * Create the integrator (integrators) needed to propagate the translational
    * and rotational state of a DynBody.
    * @param[in] generator  Integrator constructor to be used to create
    *                       state integrators.
    * @param[in] controls   The integration ontrols created the integrator
    *                       constructor's create_integration_controls method.
    * @param[in] time_mngr  The JEOD time manager object.
    */
   virtual void create_body_integrators (
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls,
      const JeodIntegrationTime & time_mngr);

   /**
    * Integrate state by the specified dynamic time interval
    * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @return The status (time advance, pass/fail status) of the integration.
    */
   virtual er7_utils::IntegratorResult integrate (
      double dyn_dt,
      unsigned int target_stage);


   // Frame switch

   /**
    * Switch the integration frame for this body and all its child bodies to the
    * indicated frame.
    *
    * @note Assumptions and Limitations@n
    *  - Limitation: Assocated frame must be a valid integration frame.
    *
    * @param[in] new_integ_frame New integration frame
    */
   virtual void switch_integration_frames (EphemerisRefFrame & new_integ_frame);

   /**
    * Switch the integration frame for this body and all its child bodies to the
    * frame indicated by the provided name.
    *
    * @note Assumptions and Limitations@n
    *  - Assumption: Provided string is a non-NULL, non-empty string.
    *  - Limitation: Assocated frame must be a valid integration frame.
    *
    * @param[in] new_integ_frame_name New integration frame
    */
   virtual void switch_integration_frames (const char * new_integ_frame_name);

   // Required by IntegrableObject, but calling this is erroroneous.
   virtual void create_integrators (
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls,
      const er7_utils::TimeInterface & time_if);

   /** Destroy the integrators. Does nothing, but must be implemented to
    * complete abstract function from the inherited IntegrableObject
    */
   virtual void destroy_integrators (void);

   // Reset the integrators.
   virtual void reset_integrators (void);

   /**
    * Find the BodyRefFrame named by the provided identifier. The name of a
    * BodyRefFrame must be prefixed by the body name. The provided identifier
    * can include or exclude this prefix. The body name is used as the prefix if
    * the the provided name does not start with the body name.
    *
    * @note Assumptions and Limitations@n
    *  - Limitation: Provided identifier must be non-NULL and non-empty.
    *     Failure to comply is a fatal error.
    *  - Limitation: The found frame must be a BodyRefFrame.
    *     Finding a non-BodyRefFrame that matches the name is a fatal error.
    *  - Assumption: Failure to find a frame is not an error.
    *     The method returns NULL if this is the case.
    *
    * @return Found frame
    * @param[in] frame_id Frame ID suffix
    */
   virtual BodyRefFrame * find_body_frame (const char * frame_id) const;

   /**
    * Get the DynamicsIntegrationGroup that integrates this DynBody object.
    * @return  Pointer to the DynamicsIntegrationGroup of this DynBody.
    */
   DynamicsIntegrationGroup * get_dynamics_integration_group();

   /**
    * Get the IntegrableObjects associated with this DynBody
    * @return A pointer to a JeodPointerVector containing the
    *         associated integrable objects.
    */
   JeodPointerVector<er7_utils::IntegrableObject>::type
   get_integrable_objects()
   {
      return associated_integrable_objects;
   }

   /**
    * Remove all IntegrableObjects associated with this DynBody.
    * You might do this if you want to switch the DynBody to a new group without
    * switching the associated IntegrableObjects.
    */
   void clear_integrable_objects();

   /**
    * Call this method before switching this dyn body to a new group
    * if you want the associated integrable objects to follow
    */
   void migrate_integrable_objects ();

   /**
    * Add an IntegrableObject to be integrated with this DynBody.
    * Note that the associated IntegrableObject may or may not follow
    * this DynBody if it is moved to a new integration group/loop.
    * @param [in] associated_integrable_object  The IntegrableObject to be
    *    associated with this DynBody.
    */
   void add_integrable_object(
      er7_utils::IntegrableObject & associated_integrable_object);

   /**
    * Remove an IntegrableObject from association with this DynBody.
    * @param [in] associated_integrable_object  The IntegrableObject to be
    *    associated with this DynBody.
    */
   void remove_integrable_object(
      er7_utils::IntegrableObject & associated_integrable_object);


   // State override

   /**
    * Set the position of the vehicle.
    *
    * @param[in] position Position wrt integ frame@n Units: M
    * @param[out] subject_frame Frame to update
    */
   void set_position (const double position[3], BodyRefFrame & subject_frame);

   /**
    * Set the velocity of the vehicle.
    *
    * @param[in] velocity Velocity wrt integ frame@n Units: M/s
    * @param[out] subject_frame Frame to update
    */
   void set_velocity (const double velocity[3], BodyRefFrame & subject_frame);

   /**
    * Set the attitude of the vehicle.
    *
    * @note Assumptions and Limitations@n
    *  - Provided quaternion is a unit quaternion.
    *
    * @param[in] left_quat Attitude wrt integ frame
    * @param[out] subject_frame Frame to update
    */
   void set_attitude_left_quaternion (
      const Quaternion left_quat, BodyRefFrame & subject_frame);

   /**
    * Set the attitude of the vehicle.
    *
    * @note Assumptions and Limitations@n
    *  - Provided quaternion is a unit quaternion.
    *
    * @param[in] right_quat Attitude wrt integ frame
    * @param[out] subject_frame Frame to update
    */
   void set_attitude_right_quaternion (
      const Quaternion right_quat, BodyRefFrame & subject_frame);

   /**
    * Set the attitude of the vehicle.
    *
    * @note Assumptions and Limitations@n
    *  - Provided matrix is orthogonal.
    *
    * @param[in] matrix Attitude wrt integ frame
    * @param[out] subject_frame Frame to update
    */
   void set_attitude_matrix (
      const double matrix[3][3], BodyRefFrame & subject_frame);

   /**
    * Set the attitude rate of the vehicle.
    *
    * @note Assumptions and Limitations@n
    *  - Provided vector is expressed in body frame coordinates.
    *
    * @param[in] attitude_rate Attitude wrt integ frame@n Units: r/s
    * @param[out] subject_frame Frame to update
    */
   void set_attitude_rate (
      const double attitude_rate[3], BodyRefFrame & subject_frame);

   /**
    * Set the parts of the specified reference frame as indicated by the
    * set_items parameter from the supplied state and propagate these items to
    * all dynamic bodies attached to this body. This method forms an integral
    * part of the state initialization process and can also be used by a
    * simulation that that receives state overrides from some other simulation.
    *
    * @note Assumptions and Limitations@n
    *  - The subject reference frame is owned by this dynamic body.
    *     This limitation is enforced.
    *
    * @param[in] set_items Items to set
    * @param[in] state State to be copied
    * @param[out] subject_frame Frame to be set
    */
   void set_state (
      RefFrameItems::Items set_items,
      const RefFrameState & state,
      BodyRefFrame & subject_frame);

   /**
    * Set the source of aspects of the state.
    * The setting is applied to the root of the DynBody tree.
    *
    * @note Assumptions and Limitations@n
    *  - The supplied frame must either be owned directly by this body or
    *     this body must be a root body and the owner of the supplied frame
    *     must be a child body of this body.
    *
    * @param[in] items Items to propagate
    * @param[in] frame Frame containing state
    */
   void set_state_source (RefFrameItems::Items items, BodyRefFrame & frame);


   // State propagation methods

   /**
    * Propagate state from the integrated state to attached bodies.
    */
   virtual void propagate_state ();

   /**
    * Propagate state from state owners to the integrated state.
    */
   virtual void update_integrated_state ();

   /**
    * Propagate structure frame state to vehicle points.
    *
    * @param[in] set_items States truly propagated
    */
   virtual void compute_vehicle_point_states (RefFrameItems::Items set_items);


   // MassBody overrides

   /**
    * Indicates whether this DynBody object is a root body.
    * @return Is this a root body?
    */
   bool is_root_body ();


   // Find this body's parent and root bodies.
   // Note that the const methods are public. The modifiable methods are not.

   /**
    * Returns this DynBody object's parent body.
    * @return Const pointer to the parent body.
    */
   virtual const DynBody * get_parent_body () const;

   /**
    * Finds this DynBody object's root body.
    * @return Const pointer to the root body.
    */
   virtual const DynBody * get_root_body () const;


   // Mass / vehicle points (mass points become vehicle points in a DynBody)

   /**
    * Add a mass point to the dyn body's list of such and
    * make a vehicle point that corresponds to the added mass point.
    *
    * @param[in] mass_point_init Mass point specification
    */
   virtual void add_mass_point (const MassPointInit & mass_point_init);

   /**
    * Find the vehicle point with the given name.
    *
    * @return Vehicle point
    * @param[in] pt_name Vehicle point name
    */
   const BodyRefFrame * find_vehicle_point (const char * pt_name) const;

   /**
    * Compute the state derivatives at a vehicle point.
    *
    * @param[in] vehicle_pt Vehicle point reference frame
    * @param[out] pt_derivs Computed derivatives
    */
   virtual void compute_vehicle_point_derivatives (
      const BodyRefFrame & frame, FrameDerivs &derivs);


   // Initialization methods

   /**
    * Indicate which state elements have been initialized.
    * @return  Initialized states indicator.
    */
   const RefFrameItems & get_initialized_states () const
   {
      return initialized_states;
   }

   /**
    * Indicate whether the specified state elements have been initialized.
    * @param  test_items  States to test.
    * @return True if all test items have been initialized, false otherwise.
    */
   bool initialized_states_contains (RefFrameItems::Items test_items) const
   {
      return initialized_states.contains (test_items);
   }

   // Attachment methods

   // Attach child mass body aligned at specified mass point
   virtual bool add_mass_body (
      const char * this_point_name,
      const char * child_point_name,
      MassBody & child);

   // Attach child mass body at geometric location relative to parent structure frame
   virtual bool add_mass_body (
      double offset[3],
      double T_pstr_cstr[3][3],
      MassBody & child );

   // Attach this body to parent body at specified mass points
   virtual bool attach_to (
      const char * this_point_name,
      const char * parent_point_name,
      DynBody & parent );

   // Attach this body to parent body at geometric location relative to parent structure frame
   virtual bool attach_to (
      double offset_pstr_cstr_pstr[3],
      double T_pstr_cstr[3][3],
      DynBody & parent );

   // Attach child body to this body at specified mass points
   virtual bool attach_child (
      const char * this_point_name,
      const char * child_point_name,
      DynBody & child );

   // Attach child body to this body at geometric location relative to this body's structure frame
   virtual bool attach_child (
      double offset_pstr_cstr_pstr[3],
      double T_pstr_cstr[3][3],
      DynBody & child );

   /**
    * Detach parent and child DynBodies, 'this' and the argument body, such
    * that the detachment happens at the parent body level. Returns true if
    * successfully detached the bodies. Returns false if unable to detach.
    * Will fail if, for example, the bodies are not in the same mass tree.
    * \par Assumptions and Limitations
    * - The detach point between non-immediate attachments (i.e. not
    *   parent/child attachments) takes place at whichever body is a
    *   progenitor. For example, a call to A.detach(D) in an A->B->C->D
    *   attachment is interpreted as a call desiring A // B->C->D. A call to
    *   D.detach(B) is interpreted as a call to A->B // C->D.
    *   @return Success flag
    *   \param[in] other_body The other body at which the detach will occur
    */
   virtual bool detach (DynBody & other_body);

   /**
    * Detach this DynBody from its parent. Equivalent to the above function
    * via detach(*dyn_parent)
    * \par Assumptions and Limitations
    * - Will inform and return false if the body has no parent.
    *   @return Success flag
    */
   virtual bool detach( void );

   /**
    * Remove connectivity between this (parent) DynBody and the
    * argument (child) MassBody mass subbody. The MassBody and associated body
    * frames are removed, such that the MassBody effectively "jettisons" from
    * dynamics operations.
    *
    * Extensibility comments --
    * - This method is invoked before the updating the parent/child
    *   states.
    * - The generic purpose of this method is to sever all connectivity
    *   links between parent and child, most importantly mass properties.
    * - Any class that overrides this method must either invoke this method
    *   or perform the actions performed herein.
    *
    * @note Assumptions and Limitations@n
    *  - The detachment must be valid or it is not performed. The MassBody
    *    must not belong to a DynBody-derived dynamic body.
    *
    * @param[in,out] child The child mass subbody; the body to be detached
    */
   virtual bool remove_mass_body (MassBody & child);


   // Member data

   /**
    * Mass properties of the vehicle, defined about the structure reference frame.
    */
   MassBody mass;

   /**
    * Body name, reference linked to mass.name
    */
   NamedItem& name; //!< trick_units(--)

   /**
    * The name of the reference frame with respect to which the body's
    * reference frames (core, composite, structure, plus vehicle point frames)
    * are to be represented and propagated. The value must identify a valid
    * integration frame, i.e., a non-rotating, ephemeris based reference frame.
    *
    * This member is used at initialization time only. To change the
    * integration frame post-initialization use the function
    * DynBody::switch_integration_frames. This can be invoked directly,
    * or indirectly via a FrameSwitch body action.
    */
   char * integ_frame_name; //!< trick_units(--)

   /**
    * The current integration frame.
    */
   EphemerisRefFrame * integ_frame; //!< trick_units(--)

   /**
    * Vehicle core body reference frame.
    * The reference frame origin is at the core body center of mass, and
    * the reference frame axes are the body frame axes as defined in the
    * core mass properties.
    */
   BodyRefFrame core_body; //!< trick_units(--)

   /**
    * Vehicle composite body reference frame.
    * The reference frame origin is at the composite body center of mass, and
    * the reference frame axes are the body frame axes as defined in the
    * composite mass properties.
    */
   BodyRefFrame composite_body; //!< trick_units(--)

   /**
    * Vehicle structural reference frame.
    * The reference frame origin is at the structural origin, and
    * the reference frame axes are the structure frame axes as defined in the
    * composite mass properties.
    */
   BodyRefFrame structure; //!< trick_units(--)

   /**
    * Is translational dynamics enabled?
    * The body's translational state is integrated only if this member is true.
    * Setting this member to false indicates the responsibility for updating
    * the translational state is performed elsewhere, such as by a
    * user-defined forced translation model.
    */
   bool translational_dynamics; //!< trick_units(--)

   /**
    * Is rotational dynamics enabled?
    * The body's rotational state is integrated only if this member is true.
    * Setting this member to false indicates the responsibility for updating
    * the rotational state is performed elsewhere, such as by a
    * user-defined forced rotation model.
    */
   bool rotational_dynamics; //!< trick_units(--)

   /**
    * Is this a three degrees of freedom (translation only) body?
    * This data member has effect only when set prior to the creation of the
    * body's integrators. The body's rotational integrator is not created
    * and rotational_dynamics is set to false if this member's value is true.

    * Note that very bad mojo (a core dump) will result if this member is
    * set to true at initialization time and rotational_dynamics is later
    * enabled during run time.
    */
   bool three_dof; //!< trick_units(--)

   /**
    * Specifies the preferred mechanism for integrating rotational state.
    * This data member has effect only when set prior to the creation of the
    * body's integrators. The body's rotational integrator will be created
    * based on the value of this data member.
    */
   GeneralizedSecondOrderODETechnique::TechniqueType
   rotation_integration; //!< trick_units(--)

   /**
    * Are vehicle points automatically updated?
    * The vehicle points are automatically calculated at initialization time
    * but are only automatically updated at runtime if this member is true.
    * Setting this member to false indicates the responsibility for updating
    * vehicle point states is performed elsewhere, such as in a
    * scheduled call to compute_vehicle_point_states.
    */
   bool autoupdate_vehicle_points; //!< trick_units(--)

   /**
    * Gravitational interactions.
    * This data member specifies how the vehicle interacts gravitationally
    * with various planetary bodies in the simulation and contains the
    * computed acceleration toward those planetary bodies.
    */
   GravityInteraction grav_interaction; //!< trick_units(--)

   /**
    * Translational/rotational accelerations.
    */
   FrameDerivs  derivs; //!< trick_units(--)


   /**
    * Force/Torque collection mechanism.
    */
   BodyForceCollect collect; //!< trick_units(--)


protected:

   // Member functions

   // Setters for the integration frame.
   // FIXME: The integration frame should be protected/private!

   /**
    * Set the integration frame for this body and all its child bodies
    * to the provided frame.
    *
    * @note Assumptions and Limitations@n
    *  - Provided frame is a valid integration frame.
    *
    * @param[in] new_integ_frame New integration frame
    */
   virtual void set_integ_frame (EphemerisRefFrame & new_integ_frame);

   /**
    * Set the integration frame for this body and all its child bodies to the
    * frame indicated by the provided name.
    *
    * @note Assumptions and Limitations@n
    *  - Assumption: Provided string is a non-NULL, non-empty string.
    *  - Assumption: State is not to be updated.
    *  - Limitation: Assocated frame must be a valid integration frame.
    *
    * @param[in] new_integ_frame_name New integration frame
    */
   virtual void set_integ_frame (const char * new_integ_frame_name);

   /**
    * Integrate the vehicle's translational state.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @return The status (time advance, pass/fail status) of the integration.
    */
   virtual er7_utils::IntegratorResult trans_integ (
      double dyn_dt, unsigned int target_stage);

   /**
    * Integrate the vehicle's rotational state.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @return The status (time advance, pass/fail status) of the integration.
    */
   virtual er7_utils::IntegratorResult rot_integ (
      double dyn_dt, unsigned int target_stage);

   // State update methods

   /**
    * Set the source of aspects of the state.
    *
    * @note Assumptions and Limitations@n
    *  - Assumptions, neither of which is checked:
    *     - This is a root body.
    *     - The supplied frame is owned by a body that is a child of this body.
    *
    * @param[in] items Items to propagate
    * @param[in] frame Frame containing state
    */
   void set_state_source_internal (
      RefFrameItems::Items items, BodyRefFrame & frame);


   // Parent / root body accessors

   /**
    * Returns this DynBody object's parent body.
    * @return Pointer to parent body.
    */
   virtual DynBody * get_parent_body_internal ();

   /**
    * Finds this DynBody object's root body.
    * @return Pointer to the root body.
    */
   virtual DynBody * get_root_body_internal ();


   // Attachment methods

   /**
    * Validate whether the pending attachment is legal from a connectivity
    * point of view.
    *
    * Extensibility comments --
    * - This method determines whether invoking attach_establish_links
    *   makes sense.
    * - Any class that overrides this method must either invoke this method
    *   or perform the actions performed herein.
    *
    * @note Assumptions and Limitations:@n
    *  - The subject body, this, must be a root body. This is not checked.
    *
    * @return Validity indicator
    * @param[in] parent The new parent body; the body to which this body is
    *                   to be attached.
    * @param[in] generate_message Generate message if invalid?
    */
   virtual bool attach_validate_parent (
      const DynBody & parent, bool generate_message) const;

   /**
    * Validate whether the pending attachment is legal from a physical
    * point of view.
    *
    * Extensibility comments --
    * - This method determines whether invoking attach_update_properties
    *   makes sense.
    *
    * @note Assumptions and Limitations@n
    *  - The subject body, child, must be a root body. This is not checked.
    *
    * @return Validity indicator
    * @param[in] child The child body; the body to be attached to this body.
    * @param[in] generate_message Generate message if invalid?
    */
   virtual bool attach_validate_child (
      const DynBody & child, bool generate_message) const;

   /**
    * Validate whether the pending sub body is legal from a mass tree
    * point of view
    *
    * @note Assumptions and Limitations@n
    *  - The subject mass, child, must not belong to a child body.
    *
    * @return Validity indicator
    * @param[in] child The child body; the body to be attached to this body.
    * @param[in] generate_message Generate message if invalid?
    */
   virtual bool add_mass_body_validate (
      const MassBody & child, bool generate_message) const;

   /**
    * For a newly attached mass sub-body, create body frames for the root
    * sub-body and all child sub-bodies via recursion.
    *
    * @return Validity indicator
    * @param[in] subbody  the root of the newly attached sub-bodies
    */
   virtual void add_mass_body_frames ( MassBody & subbody );

  /**
   * For a newly detached mass sub-body, remove body frames for the root
   * sub-body and all child sub-bodies via recursion.
   *
   * @return Validity indicator
   * @param[in] subbody  the root of the newly attached sub-bodies
   */
   virtual void detach_mass_body_frames ( MassBody & subbody );

   /**
    * Establish the logical connectivity between parent and child.
    *
    * Extensibility comments --
    * - This method is invoked before the computing the physical relation
    *   between parent and child.
    * - The generic purpose of this method is to establish the logical
    *   connectivity between parent and child in terms of the child class.
    * - Any class that overrides this method must either invoke this
    *   method or perform the actions performed herein.
    *
    * @note Assumptions and Limitations@n
    *  - The attachment is valid; not checked.
    *
    * @param[in,out] parent The new parent body;
    *                       the body to which this body is to be attached.
    */
   virtual void attach_establish_links (DynBody & parent);

   /**
    * Set the relation between parent and child and update the mass
    * properties.
    *
    * Extensibility comments --
    * - This method is sent to the parent body of the attachment after
    *   the child body has established the logical connectivity between
    *   the parent body and child body.
    * - The generic purpose of this method is to establish the physical
    *   relation between parent and child and to update any physical
    *   properties that change as a result of the attachment.
    * - Any class that overrides this method must either invoke this method
    *   or perform the actions performed herein.
    *
    * @note Assumptions and Limitations@n
    *  - The attachment is valid
    *  - Logical connectivity has been established.
    *  .
    *  Neither assumption is checked.
    *
    * @param[in] offset_pstr_cstr_pstr Location of this body's structural origin
    *            with respect to the new parent body's structural origin,
    *            specified in structural coordinates of the new parent body.@n
    *            Units: m
    * @param[in] T_pstr_cstr Transformation matrix from the new parent body's
    *            structural frame to this body's structural frame.
    * @param[in,out] child The child body; the body newly attached to this body.
    */
   virtual void attach_update_properties (
      double offset_pstr_cstr_pstr[3],
      double T_pstr_cstr[3][3],
      DynBody & child);

   /**
    * Process the attachment event of one body from another.
    * This method is called by the attach method after the links have
    * established or severed and is invoked twice:
    * - On the parent, in which case the parent argument is null and the child
    *   argument is the child that attached from the parent, and
    * - On the detaching child, in which case the child argument is null and the
    *   parent argument is the body from which the child was detached.
    *
    * @note Assumptions and Limitations:@n
    *  - Instances of more derived classes, with presumably more involved
    *    dynamics, are situated higher in the mass tree than are more basic
    *    instances. For example, a simple MassBody can be a child of a DynBody,
    *    but not the other way around.
    *  - The attachment in the mass tree between the immediate child and the
    *    superior body is assumed to reflect a real physical attachment.
    *
    * @param[in] offset_pstr_cstr_pstr Location of this body's structural origin
    *            with respect to the new parent body's structural origin,
    *            specified in structural coordinates of the new parent body.@n
    *            Units: m
    * @param[in] T_pstr_cstr Transformation matrix from the new parent body's
    *            structural frame to this body's structural frame.
    * @param[in,out] root_body Body at the root of the mass tree
    * @param[in,out] child_body Body that is being attached to this body.
    */
   virtual void process_dynamic_attachment (
      double offset_pstr_cstr_pstr[3],
      double T_pstr_cstr[3][3],
      DynBody & root_body,
      DynBody & child_body);

   /**
    * Update parent and child properties to reflect that they are
    * detached.
    *
    * Extensibility comments --
    * - This method is sent to the parent body of the detachment after
    *   the child body has severed the logical connectivity between
    *   the parent body and child body.
    * - The generic purpose of this method is to update any physical
    *   properties that change as a result of the detachment.
    * - Any class that overrides this method must either invoke this method
    *   or perform the actions performed herein.
    *
    * @note Assumptions and Limitations@n
    *  - The detachment is valid and logical connectivity has been severed.
    *     Neither assumption is checked.
    *
    * @param[in,out] child The child body; the body newly detached from this
    *                body.
    */
   virtual void detach_mass_internal (MassBody & child);


   // State propagation methods

   /**
    * Propagate state to attached bodies starting from this body's
    * structural frame.
    *
    * @note Assumptions and Limitations@n
    *  - At least some states are set.
    */
   virtual void propagate_state_from_structure ();

   /**
    * Propagate state to attached bodies starting from this body's
    * composite frame.
    *
    * @note Assumptions and Limitations@n
    *  - At least some states are set.
    */
   virtual void propagate_state_from_composite ();

   /**
    * Compute the relative state between the integrated frame's mass point
    * and the source frame's mass point.
    *
    * @note Assumptions and Limitations@n
    *  - This method is only called to be called for a root body.
    *     This assumption is not enforced.
    *
    * @param[in] source_frame The frame that contains the relevant state data.
    * @param[in,out] ref_point The mass point corresponding to the previous call
    *                to this function. This is an efficiency hack used to avoid
    *                duplicative computations.
    * @param[in,out] rel_state The relative state between the integration frame
    *                mass point and the source frame mass point.
    */
   void compute_ref_point_transform (
      const BodyRefFrame & source_frame,
      const MassPoint ** const ref_point,
      MassPointState & rel_state);

   /**
    * Compute a derived state given the source state and the position/
    * attitude transformation from the source to the derived state.
    *
    * @param[in] source_frame Source state
    * @param[in] rel_state Relative state
    * @param[out] derived_frame Derived state
    */
   void compute_derived_state_forward (
      const BodyRefFrame & source_frame,
      const MassPoint & rel_state,
      BodyRefFrame & derived_frame) const;

   /**
    * Compute selected aspects of the derived state given the source state
    * and the position/ attitude transformation from the source to the
    * derived state.
    *
    * @param[in] source_frame Source state
    * @param[in] rel_state Relative state
    * @param[in] state_items States to compute
    * @param[out] derived_frame Derived state
    */
   void compute_state_elements_forward (
      const BodyRefFrame & source_frame,
      const MassPoint & rel_state,
      const RefFrameItems & state_items,
      BodyRefFrame & derived_frame) const;

   /**
    * Compute a derived state given the source state and the position/
    * attitude transformation from the derived to the source state.
    *
    * @param[in] source_frame Source state
    * @param[in] rel_state Relative state
    * @param[out] derived_frame Derived state
    **/
   void compute_derived_state_reverse (
      const BodyRefFrame & source_frame,
      const MassPoint & rel_state,
      BodyRefFrame & derived_frame) const;

   /**
    * Compute selected aspects of the derived state given the source state
    * and the position/ attitude transformation from the derived to the
    * source state.
    *
    * @param[in] source_frame Source state
    * @param[in] rel_state Relative state
    * @param[in] state_items States to compute
    * @param[out] derived_frame Derived state
    */
   void compute_state_elements_reverse (
      const BodyRefFrame & source_frame,
      const MassPoint & rel_state,
      const RefFrameItems & state_items,
      BodyRefFrame & derived_frame) const;


   // Member data

   /**
    * The dynamics manager for the simulation.
    */
   BaseDynManager*& dyn_manager; //!< trick_units(--)

   /**
    * The time manager to be used to obtain timestamp information.
    */
   const JeodIntegrationTime * time_manager; //!< trick_units(--)

   /**
    * The DynBody to which this body is attached. This points to exactly the
    * same object as does the links.parent member. While a mass body can be
    * attached to any kind of mass body, a dynamic body can only be attached
    * to another dynamic body.
    */
   DynBody * dyn_parent; //!< trick_units(--)

   /**
    * The subset of the dynamic bodies attached to this dynamic body
    */
   std::list<DynBody*> dyn_children;

   /**
    * The subset of the mass bodies attached to this dynamic body that are
    * themselves not dynamic bodies.
    */
   std::list<MassBody*> mass_children;

   /**
    * An array of vehicle points associated with this dynamic body.
    */
   std::list<BodyRefFrame*> vehicle_points;

   /**
    * Enum value indicating which of position, velocity, attitude, and rate
    * have been initialized.
    */
   RefFrameItems initialized_states; //!< trick_units(--)

   /**
    * The reference frame that contains the user-set position.
    */
   BodyRefFrame * position_source; //!< trick_units(--)

   /**
    * The reference frame that contains the user-set velocity.
    */
   BodyRefFrame * velocity_source; //!< trick_units(--)

   /**
    * The reference frame that contains the user-set attitude.
    */
   BodyRefFrame * attitude_source; //!< trick_units(--)

   /**
    * The reference frame that contains the user-set attitude rate.
    */
   BodyRefFrame * rate_source; //!< trick_units(--)

   /**
    * The reference frame whose state is updated via the state integrator.
    * All other reference frames are calculated from this frame.
    */
   BodyRefFrame * integrated_frame; //!< trick_units(--)

   /**
    * List of integrable objects to be integrated
    * with this DynBody
    */
   std::vector<er7_utils::IntegrableObject*>
      associated_integrable_objects;  //!< trick_io(**)

   /**
    * The object that merges integration results.
    */
   er7_utils::IntegratorResultMergerContainer
      integ_results_merger; //!< trick_units(--)

   /**
    * Translational state checkpointable/restartable integrator generator.
    * Translational state is comparatively easy to integrate.
    * The canonical position is just position, canonical velocity is just
    * velocity, and the time derivative of position is velocity.
    */
   RestartableT3SecondOrderODEIntegrator trans_integrator; //!< trick_units(--)

   /**
    * Rotational state checkpointable/restartable integrator generator.
    * Rotational state is much harder to integrate.
    * The canonical position is the attitude quaternion, canonical velocity is
    * angular velocity, and the time derivative of the attitude quaternion is
    * a function of the orientiation and the angular velocity.
    */
   RestartableSO3SecondOrderODEIntegrator rot_integrator; //!< trick_units(--)


private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   DynBody (const DynBody &);

   /**
    * Not implemented.
    */
   DynBody & operator= (const DynBody &);
};

} // End JEOD namespace


#endif

/**
 * @}
 * @}
 * @}
 */
