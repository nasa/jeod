//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/include/restartable_state_integrator.hh
 * Define classes that encapsulate the construction, destruction,
 * checkpointing, and restarting of state integrators.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/


#ifndef JEOD_RESTARTABLE_INTEGRATOR_HH
#define JEOD_RESTARTABLE_INTEGRATOR_HH

// Local includes
#include "restartable_state_integrator_templates.hh"
#include "generalized_second_order_ode_technique.hh"
#include "integration_messages.hh"

// JEOD includes
#include "utils/container/include/simple_checkpointable.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/first_order_ode_integrator.hh"
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"
#include "er7_utils/integration/core/include/left_quaternion_functions.hh"

// System includes
#include <cstddef>


//! Namespace jeod
namespace jeod {

/**
 * A RestartableScalarFirstOrderODEIntegrator integrates a first order ODE,
 *  dx/dt = v(x,t), where x is a scalar.
 */
class RestartableScalarFirstOrderODEIntegrator : public SimpleCheckpointable {
   JEOD_MAKE_SIM_INTERFACES(RestartableScalarFirstOrderODEIntegrator)

public:

   /**
    * Default constructor.
    */
   RestartableScalarFirstOrderODEIntegrator ()
   :
      SimpleCheckpointable(),
      integrator(nullptr),
      integrator_manager(integrator)
   {
      JEOD_REGISTER_CLASS (RestartableScalarFirstOrderODEIntegrator);
   }

   /**
    * Destructor.
    */
   ~RestartableScalarFirstOrderODEIntegrator () override { }

   /**
    * Create the integrator to be managed.
    * @param[in] generator  Integrator constructor used to create the integrator.
    * @param[in,out] controls   Integration controls to be passed to the generator.
    */
   void create_integrator (
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls)
   {
      integrator_manager.create_integrator (generator, controls);
   }

   /**
    * Destroy the integrator.
    */
   void destroy_integrator ()
   {
      integrator_manager.destroy_integrator();
   }

   /**
    * Propagate state to the specified stage of the integration
    * process for an overall integration time interval of dyn_dt.
    *
    * Note that this is a pass-through to the encapsulated integrator object.
    * See er7_utils::FirstOrderODEIntegrator::integrate for details.
    *
    * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @param[in]     xdot          Time derivative of x.
    * @param[in,out] x             Item to be integrated.
    *
    * @return The status (time advance, pass/fail status) of the integration.
    */
   er7_utils::IntegratorResult integrate (
      double dyn_dt,
      unsigned int target_stage,
      double * ER7_UTILS_RESTRICT xdot,
      double * ER7_UTILS_RESTRICT x)
   ER7_UTILS_ALWAYS_INLINE
   {
      return integrator->integrate (dyn_dt, target_stage, xdot, x);
   }

   /**
    * Tell the integrator to reset itself. This should be called when the time
    * step or time direction changes or upon a discrete change in state such
    * docking/undocking. Such events invalidate saved state (if any).
    * The integrator needs to restart from scratch when such events occur.
    */
   void reset_integrator ()
   {
      integrator->reset_integrator();
   }

   /**
    * Restore the integrator on restart.
    */
   void simple_restore () override
   {
      integrator_manager.set_integrator_reference (integrator);
      integrator_manager.simple_restore ();
   }


private:

   /**
    * Pointer to the object that performs integration. The object is created
    * managed by the integrator manager.
    */
   er7_utils::FirstOrderODEIntegrator * integrator; //!< trick_units(--)

   /**
    * Object that creates and manages the integrator object.
    */
   RestartableFirstOrderODEIntegrator<1> integrator_manager; //!< trick_io(**)


   // Unimplemented member functions

   /**
    * Not implemented.
    */
   RestartableScalarFirstOrderODEIntegrator (
      const RestartableScalarFirstOrderODEIntegrator &);

   /**
    * Not implemented.
    */
   RestartableScalarFirstOrderODEIntegrator & operator= (
      const RestartableScalarFirstOrderODEIntegrator &);
};


/**
 * A RestartableT3SecondOrderODEIntegrator integrates a second order ODE
 * in three space, d^2x/dt^2 = a(x,t), where x is a three-vector.
 */
class RestartableT3SecondOrderODEIntegrator : public SimpleCheckpointable {
   JEOD_MAKE_SIM_INTERFACES(RestartableT3SecondOrderODEIntegrator)

public:

   /**
    * Default constructor.
    */
   RestartableT3SecondOrderODEIntegrator ()
   :
      SimpleCheckpointable(),
      integrator(nullptr),
      integrator_manager(integrator)
   {
      JEOD_REGISTER_CLASS (RestartableT3SecondOrderODEIntegrator);
   }

   /**
    * Destructor.
    */
   ~RestartableT3SecondOrderODEIntegrator () override { }

   /**
    * Create the integrator to be managed.
    * @param[in] generator  Integrator constructor used to create the integrator.
    * @param[in,out] controls   Integration controls to be passed to the generator.
    */
   void create_integrator (
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls)
   {
      integrator_manager.create_integrator (generator, controls);
   }

   /**
    * Destroy the integrator.
    */
   void destroy_integrator ()
   {
      integrator_manager.destroy_integrator();
   }

   /**
    * Propagate state to the specified stage of the integration
    * process for an overall integration time interval of dyn_dt.
    *
    * Note that this is a pass-through to the encapsulated integrator object.
    * See er7_utils::SecondOrderODEIntegrator::integrate for details.
    *
    * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @param[in]     accel         Time derivative of the generalized velocity.
    * @param[in,out] velocity      Generalized velocity vector.
    * @param[in,out] position      Generalized position vector.
    *
    * @return The status (time advance, pass/fail status) of the integration.
    */
   er7_utils::IntegratorResult integrate (
      double dyn_dt,
      unsigned int target_stage,
      double const * ER7_UTILS_RESTRICT accel,
      double * ER7_UTILS_RESTRICT velocity,
      double * ER7_UTILS_RESTRICT position)
   ER7_UTILS_ALWAYS_INLINE
   {
      return integrator->integrate (dyn_dt, target_stage, accel,
                                    velocity, position);
   }

   /**
    * Tell the integrator to reset itself. This should be called when the time
    * step or time direction changes or upon a discrete change in state such
    * docking/undocking. Such events invalidate saved state (if any).
    * The integrator needs to restart from scratch when such events occur.
    */
   void reset_integrator ()
   {
      integrator->reset_integrator();
   }

   /**
    * Restore the integrator on restart.
    */
   void simple_restore () override
   {
      integrator_manager.set_integrator_reference (integrator);
      integrator_manager.simple_restore ();
   }


private:

   /**
    * Pointer to the object that performs integration. The object is created
    * managed by the integrator manager.
    */
   er7_utils::SecondOrderODEIntegrator * integrator; //!< trick_units(--)

   /**
    * Object that creates and manages the integrator object.
    */
   RestartableSimpleSecondOrderODEIntegrator<3>
      integrator_manager; //!< trick_io(**)


   // Unimplemented member functions

   /**
    * Not implemented.
    */
   RestartableT3SecondOrderODEIntegrator (
      const RestartableT3SecondOrderODEIntegrator &);

   /**
    * Not implemented.
    */
   RestartableT3SecondOrderODEIntegrator & operator= (
      const RestartableT3SecondOrderODEIntegrator &);
};


/**
 * A RestartableSO3SecondOrderODEIntegrator integrates a generalized second
 * order ODE that describes rotation in three space.
 */
class RestartableSO3SecondOrderODEIntegrator : public SimpleCheckpointable {
   JEOD_MAKE_SIM_INTERFACES(RestartableSO3SecondOrderODEIntegrator)

public:

   /**
    * Default constructor.
    */
   RestartableSO3SecondOrderODEIntegrator ()
   :
      SimpleCheckpointable(),
      technique(GeneralizedSecondOrderODETechnique::Unspecified),
      integrator(nullptr),
      generalized_deriv_integrator_manager(),
      generalized_step_integrator_manager()
   {
      JEOD_REGISTER_CLASS (RestartableSO3SecondOrderODEIntegrator);
   }

   /**
    * Destructor.
    */
   ~RestartableSO3SecondOrderODEIntegrator () override
   {
      destroy_integrator();
   }

   /**
    * Create the integrator to be managed.
    * @param[in] technique_in  Integration technique; generalized step vs deriv.
    * @param[in] generator     Integrator constructor used to create the integrator.
    * @param[in,out] controls      Integration controls to be passed to the generator.
    */
   void create_integrator (
      GeneralizedSecondOrderODETechnique::TechniqueType technique_in,
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls)
   {
      // Save the integration technique for checkpoint/restart.
      technique = technique_in;

      // Generate the integration generator.
      if (technique == GeneralizedSecondOrderODETechnique::Cartesian) {
         generalized_deriv_integrator_manager.set_integrator_reference (
            integrator);
         generalized_deriv_integrator_manager.create_integrator (
            generator, controls);
      }
      else if (technique == GeneralizedSecondOrderODETechnique::LieGroup) {
         generalized_step_integrator_manager.set_integrator_reference (
            integrator);
         generalized_step_integrator_manager.create_integrator (
            generator, controls);
      }
      else {
         MessageHandler::fail (
            __FILE__, __LINE__, IntegrationMessages::invalid_item,
            "The integration option is unspecified.");
      }
   }

   /**
    * Destroy the integrator.
    */
   void destroy_integrator ()
   {
      generalized_deriv_integrator_manager.destroy_integrator();
      generalized_step_integrator_manager.destroy_integrator();

      technique = GeneralizedSecondOrderODETechnique::Unspecified;
   }

   /**
    * Propagate state to the specified stage of the integration
    * process for an overall integration time interval of dyn_dt.
    *
    * Note that this is a pass-through to the encapsulated integrator object.
    * See er7_utils::SecondOrderODEIntegrator::integrate for details.
    *
    * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @param[in]     accel         Time derivative of the generalized velocity.
    * @param[in,out] velocity      Generalized velocity vector.
    * @param[in,out] position      Generalized position vector.
    *
    * @return The status (time advance, pass/fail status) of the integration.
    */
   er7_utils::IntegratorResult integrate (
      double dyn_dt,
      unsigned int target_stage,
      double const * ER7_UTILS_RESTRICT accel,
      double * ER7_UTILS_RESTRICT velocity,
      double * ER7_UTILS_RESTRICT position)
   ER7_UTILS_ALWAYS_INLINE
   {
      return integrator->integrate (dyn_dt, target_stage, accel,
                                    velocity, position);
   }

   /**
    * Tell the integrator to reset itself. This should be called when the time
    * step or time direction changes or upon a discrete change in state such
    * docking/undocking. Such events invalidate saved state (if any).
    * The integrator needs to restart from scratch when such events occur.
    */
   void reset_integrator ()
   {
      integrator->reset_integrator();
   }

   /**
    * Restore the integrator on restart.
    */
   void simple_restore () override
   {
      generalized_step_integrator_manager.clear_integrator_reference ();
      generalized_deriv_integrator_manager.clear_integrator_reference ();

      // Restore the integrator from the checkpoint-restored value in the
      // external pointer and perform technique-specific actions on it.
      if (technique == GeneralizedSecondOrderODETechnique::Cartesian) {
         generalized_deriv_integrator_manager.set_integrator_reference (
            integrator);
         generalized_deriv_integrator_manager.simple_restore ();
      }
      else if (technique == GeneralizedSecondOrderODETechnique::LieGroup) {
         generalized_step_integrator_manager.set_integrator_reference (
            integrator);
         generalized_step_integrator_manager.simple_restore ();
      }
   }


private:

   /**
    * Specifies the mechanism for integrating rotational state.
    */
   GeneralizedSecondOrderODETechnique::TechniqueType
      technique; //!< trick_units(--)

   /**
    * Pointer to the object that performs integration.
    * This object is created and managed by one of the integrator managers
    * defined below. The techique dictates which of the two is used.
    */
   er7_utils::SecondOrderODEIntegrator * integrator; //!< trick_units(--)

   /**
    * SO3 generalized derivative integrator.
    */
   RestartableGeneralizedDerivSecondOrderODEIntegrator<
         4, 3, er7_utils::LeftQuaternionGeneralizedPositionFunctions>
      generalized_deriv_integrator_manager; //!< trick_io(**)

   /**
    * SO3 Lie Group integrator.
    */
   RestartableGeneralizedStepSecondOrderODEIntegrator<
         4, 3, er7_utils::LeftQuaternionGeneralizedPositionFunctions>
      generalized_step_integrator_manager; //!< trick_io(**)


   // Unimplemented member functions

   /**
    * Not implemented.
    */
   RestartableSO3SecondOrderODEIntegrator (
      const RestartableSO3SecondOrderODEIntegrator &);

   /**
    * Not implemented.
    */
   RestartableSO3SecondOrderODEIntegrator & operator= (
      const RestartableSO3SecondOrderODEIntegrator &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
