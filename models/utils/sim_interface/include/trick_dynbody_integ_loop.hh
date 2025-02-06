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
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/trick_dynbody_integ_loop.hh
 * Define the class IntegrationGroupIntegLoopScheduler,
 * which replaces the base Trick integration loop for multi-rate JEOD-based
 * simulations.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((Take great care in mixing Trick integration and JEOD integration.))

Library dependencies:
  ((trick_dynbody_integ_loop.o))

 

*******************************************************************************/


#ifndef JEOD_DYNBODY_INTEG_LOOP_SCHEDULER_HH
#define JEOD_DYNBODY_INTEG_LOOP_SCHEDULER_HH

#ifdef TRICK_VER

// Local includes
#include "jeod_trick_integrator.hh"

// JEOD includes
#include "dynamics/dyn_manager/include/dynamics_integration_group.hh"
#include "utils/integration/include/jeod_integration_group.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Trick includes
#include "sim_services/Integrator/include/IntegLoopScheduler.hh"


//! Namespace jeod
namespace jeod {
// Forward declarations
class DynManager;
class DynamicsIntegrationGroup;
class TimeManager;
class GravityManager;
} // End JEOD namespace

/**
 * Namespace Trick furnishes several standard functions for use in the Trick environment.
 */
namespace Trick {
class SimObject;
}

/**
 * Namespace er7_utils contains the state integration models used by JEOD.
 */
namespace er7_utils {
class IntegrableObject;
}


//! Namespace jeod
namespace jeod {

/**
 * A Trick::IntegLoopScheduler that provides the ability to integrate a
 * collection of Trick::SimObject instances over time, with the sim objects
 * capable of being moved from one integration loop to another during run time.
 * A JeodDynbodyIntegrationLoop augments this capability in a number of regards:
 * - All DynBody objects contained in the sim objects integrated by a
 *   JeodDynbodyIntegrationLoop object are integrated using JEOD integration.
 * - The DynBody objects to be integrated by a JeodDynbodyIntegrationLoop object
 *   are automatically collected as a member of the DynamicsIntegrationGroup
 *   object contained within a JeodDynbodyIntegrationLoop object.
 * - Non-DynBody integrable objects can also be integrated using
 *   JEOD integration.
 * - Non-DynBody integrable objects that are elsewhere identified as being
 *   associated with a DynBody object are automatically collected along with
 *   the DynBody objects with which they are associated.
 * - The DynBody and associated integrable objects are integrated using the
 *   DynamicsIntegrationGroup object contained in the loop object.
 *
 * Users of this class are strongly encouraged to do so via a
 * JeodIntegLoopSimObject.
 * See $JEOD_HOME/lib/jeod/JEOD_S_modules/integ_loop.sm.
 */
class JeodDynbodyIntegrationLoop :
   public Trick::IntegLoopScheduler,
   virtual public JeodIntegrationGroupOwner
{

   JEOD_MAKE_SIM_INTERFACES(JeodDynbodyIntegrationLoop)

public:

   // Member functions

   // Constructor and destructor

   /**
    * JeodDynbodyIntegrationLoop default constructor.
    * @note This exists only for the purpose of automated checkpoint/restart.
    * @warning Do not use the default constructor outside of this context.
    */
   JeodDynbodyIntegrationLoop ();


   /**
    * JeodDynbodyIntegrationLoop non-default constructor.
    * This is the constructor that should be used in the S_define file.
    * The SimObject that contains this JeodDynbodyIntegrationLoop instance must
    * register an "integ_loop" class job that calls the loop's integrate method.
    * @param cycle
    *   The integration interval in simulation seconds. This must be the same
    *   interval as specified in the integ_loop job specification.
    * @param sim_object_in
    *   The SimObject that contains this JeodDynbodyIntegrationLoop instance.
    * @param time_manager_in
    *   The simulation's time manager object.
    * @param dyn_manager_in
    *   The simulation's dynamics manager object.
    * @param grav_manager_in
    *   The simulation's gravity manager object.
    * @param integ_cotr_in
    *   The integrator constructor used to create integration artifacts.
    * @param integ_group_factory
    *   The integration group object used to create this loop's integ group.
    */
   JeodDynbodyIntegrationLoop (
      double cycle,
      Trick::SimObject & sim_object_in,
      TimeManager & time_manager_in,
      DynManager & dyn_manager_in,
      GravityManager & grav_manager_in,
      er7_utils::IntegratorConstructor *& integ_cotr_in,
      DynamicsIntegrationGroup & integ_group_factory);


   /**
    * JeodDynbodyIntegrationLoop destructor.
    */
   virtual ~JeodDynbodyIntegrationLoop ();


   // Member functions

   /**
    * S_define-level function to initialize the integration loop.
    *
    * This function should be called as a very low phase integration class job.
    */
   void initialize_integ_loop (void);


   /**
    * S_define-level function to reset JEOD time to the time at the start of
    * the current integration loop.
    *
    * This function should be called as a very low phase pre-integration class
    * job in simulations that have multiple integration loops.
    */
   void set_time_to_loop_start();


   /**
    * Update the provided integration group, which must be the integration group
    * contained within this integration loop object.
    *
    * @note This function is public because it is called (indirectly) from
    * DynManager::initialize_simulation. It should otherwise be viewed
    * as a protected or private function.
    *
    * @param group  The IntegrationGroup to be updated, which must be
    *               the integration loop's integration group object.
    */
   virtual void update_integration_group (
      JeodIntegrationGroup & group);


   /**
    * Add a sim object to the set of objects to be integrated by this
    * integration loop object. The job queues for this loop are rebuilt after
    * adding the sim object.
    *
    * @param sim_obj  The SimObject to be added to this loop object.
    * @return  Zero => success, non-zero => error.
    */
   virtual int add_sim_object (Trick::SimObject & sim_obj);


   /**
    * Add the specified integrable object, which should not be a DynBody,
    * to the integration group's set of integrable objects.
    *
    * @param integrable_object  Object to be added.
    */
   virtual void add_integrable_object (
      er7_utils::IntegrableObject & integrable_object);


   /**
    * Remove a sim object from the set of objects to be integrated by this
    * integration loop object. The job queues for this loop are rebuilt after
    * removing the sim object.
    *
    * @param sim_obj  The SimObject to be removed from this loop object.
    * @return  Zero => success, non-zero => error.
    */
   virtual int remove_sim_object (Trick::SimObject & sim_obj);


   /**
    * Remove the specified integrable object from the integration group's
    * set of integrable objects.
    *
    * @param integrable_object  Object to be removed.
    */
   virtual void remove_integrable_object (
      er7_utils::IntegrableObject & integrable_object);


   // Passthru methods

   /**
    * Compute the gravitational accelerations of each dynamic body
    * that is integrated by this integration loop.
    */
   virtual void gravitation (void)
   {
      integ_group->gravitation (*dyn_manager, *gravity_manager);
   }


   /**
    * Collect the derivatives for each dynamic body
    * that is integrated by this integration loop.
    */
   virtual void collect_derivatives (void)
   {
      integ_group->collect_derivatives ();
   }


   /**
    * Set the deriv_ephem_update flag for the integration group.
    * @param val  New value for deriv_ephem_update.
    */
   virtual void set_deriv_ephem_update (bool val)
   {
      deriv_ephem_update = val;
      if (integ_group != NULL) {
         integ_group->deriv_ephem_update = deriv_ephem_update;
      }
   }


protected:

   // Member functions

   /**
    * Find the sim object that contains the specified integrable object.
    * @param integrable_object Object to be found.
    * @return Sim object that contains the specified object, or null if none.
    */
   Trick::SimObject * find_containing_sim_object (
      er7_utils::IntegrableObject & integrable_object);


   /**
    * Add the DynBody objects contained in the specified sim object to the set
    * of DynBody objects integrated by this integration loop.
    *
    * @param sim_obj  The SimObject being added to this loop object.
    */
   virtual void add_sim_object_bodies (Trick::SimObject & sim_obj);


   /**
    * Add the dyn bodies contained in all the sim objects integrated by
    * this integration loop to the loop's integration group.
    */
   virtual void add_sim_object_bodies (void);


   /**
    * Remove the DynBody objects contained in the specified sim object from the
    * set of DynBody objects integrated by this integration loop.
    *
    * @param sim_obj  The SimObject being removed from this loop object.
    */
   virtual void remove_sim_object_bodies (Trick::SimObject & sim_obj);


   /**
    * Integrate sim objects over the specified time span.
    * This is an overridable internal integration function and is called by the
    * externally-visible integrate method and by call_dynamic_event_jobs.
    *
    * @return               Zero/non-zero success indicator.
    *                       Out-of-sync integrators cause a non-zero return.
    * @param beg_sim_time   The time at the start of the integration interval.
    * @param del_sim_time   The time span of the integration interval.
    */
   virtual int integrate_dt (double beg_sim_time, double del_sim_time);


   // Member data

   /**
    * The simulation object that contains this integration loop object.
    */
   Trick::SimObject * loop_sim_object; //!< trick_units(--)

   /**
    * The JEOD dynamics manager.
    */
   DynManager * dyn_manager; //!< trick_units(--)

   /**
    * The JEOD time manager.
    */
   TimeManager * time_manager; //!< trick_units(--)

   /**
    * The gravity model manager.
    */
   GravityManager * gravity_manager; //!< trick_units(--)

   /**
    * Dummy integration interface; needed by the integ_group.
    */
   JeodTrickIntegrator integ_interface; //!< trick_units(--)

   /**
    * Handle to the integration constructor used to create integrators.
    */
   er7_utils::IntegratorConstructor ** integ_constructor; //!< trick_units(--)

   /**
    * The externally-supplied integration group used as a template for
    * creating this integration loop's integration group.
    */
   const DynamicsIntegrationGroup * integ_group_factory; //!< trick_units(--)

   /**
    * The integration group that performs  the integration.
    */
   DynamicsIntegrationGroup * integ_group; //!< trick_units(--)

   /**
    * If set, ephemerides will be updated at the derivative rate.
    * If clear, ephemerides will not be updated at the derivative rate by
    * the ephemerides manager. Derivative-rate updates can still be attained
    * by explicitly calling the various ephemerides model's update functions
    * as derivative class jobs.
    */
   bool deriv_ephem_update; //!< trick_units(--)


private:
   // Having a copy constructor and assignment operator doesn't make a lick of
   // sense for this class.

   //!< Deleted.
   JeodDynbodyIntegrationLoop (const JeodDynbodyIntegrationLoop &);

   //!< Deleted.
   JeodDynbodyIntegrationLoop& operator= (const JeodDynbodyIntegrationLoop &);
};


} // End JEOD namespace

#endif

#endif

/**
 * @}
 * @}
 * @}
 */
