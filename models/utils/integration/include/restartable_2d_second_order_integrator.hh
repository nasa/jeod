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
 * @file
 * Defines the class Restartable2DSecondOrderODEIntegrator.
 */

/*
Purpose: ()
*/


#ifndef JEOD_RESTARTABLE_2D_SECOND_ORDER_INTEGRATOR_HH
#define JEOD_RESTARTABLE_2D_SECOND_ORDER_INTEGRATOR_HH


// JEOD includes
#include "restartable_state_integrator_templates.hh"
#include "generalized_second_order_ode_technique.hh"
#include "integration_messages.hh"

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
 * Integrates a second order ODE in two dimensional space,
 * d^2x/dt^2 = a(x,t), where x is a two-vector.
 */
class Restartable2DSecondOrderIntegrator : public SimpleCheckpointable
{
   JEOD_MAKE_SIM_INTERFACES(Restartable2DSecondOrderIntegrator)

public:

   /**
    * Default constructor.
    */
   Restartable2DSecondOrderIntegrator()
   :
      SimpleCheckpointable(),
      integrator(nullptr),
      integrator_manager(integrator)
   {
      JEOD_REGISTER_CLASS (Restartable2DSecondOrderIntegrator);
   }

   /**
    * Destructor.
    */
   ~Restartable2DSecondOrderIntegrator() override
   {
      destroy_integrator();
   }


   /**
    * Create the integrator to be managed.
    * @param[in] generator  Generator used to create the integrator.
    * @param[in,out] controls  Controls to be passed to the generator.
    */
   void create_integrator(
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls)
   {
      integrator_manager.create_integrator (generator, controls);
   }

   /**
    * Destroy the integrator.
    */
   void destroy_integrator()
   {
      integrator_manager.destroy_integrator();
      integrator_manager.set_integrator_reference (integrator);
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
   er7_utils::IntegratorResult integrate(
      double dyn_dt,
      unsigned int target_stage,
      double const * ER7_UTILS_RESTRICT accel,
      double * ER7_UTILS_RESTRICT velocity,
      double * ER7_UTILS_RESTRICT position)
   ER7_UTILS_ALWAYS_INLINE
   {
      return integrator->integrate(dyn_dt, target_stage, accel,
                                   velocity, position);
   }

   /**
    * Tell the integrator to reset itself. This should be called when the time
    * step or time direction changes or upon a discrete change in state such
    * docking/undocking. Such events invalidate saved state (if any).
    * The integrator needs to restart from scratch when such events occur.
    */
   void reset_integrator()
   {
      integrator->reset_integrator();
   }

   /**
    * Restore the integrator on restart.
    */
   void simple_restore() override
   {
      integrator_manager.set_integrator_reference(integrator);
      integrator_manager.simple_restore();
   }


private:

   /**
    * The pointer to the object that performs integration.
    * This object is created managed by the integrator manager.
    */
   er7_utils::SecondOrderODEIntegrator * integrator; //!< trick_units(--)

   /**
    * The object that creates and manages the integrator object.
    */
   RestartableSimpleSecondOrderODEIntegrator<2>
      integrator_manager; //!< trick_io(**)


   // Unimplemented member functions

   /**
    * Not implemented.
    */
   Restartable2DSecondOrderIntegrator(
      const Restartable2DSecondOrderIntegrator &);

   /**
    * Not implemented.
    */
   Restartable2DSecondOrderIntegrator & operator=(
      const Restartable2DSecondOrderIntegrator &);
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
