/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup Lsode
 * @{
 *
 * @file models/utils/integration/lsode/src/lsode_integration_controls.cc
 * Define the methods for the class LsodeIntegrationControls
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (())

Library dependencies:
  ((lsode_integration_controls.o))

 

*******************************************************************************/


// Integration includes
#include "er7_utils/integration/core/include/base_integration_group.hh"
#include "er7_utils/integration/core/include/integrator_interface.hh"
#include "er7_utils/integration/core/include/time_interface.hh"
#include "er7_utils/interface/include/message_handler.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"

/* JEOD includes */
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/lsode_integration_controls.hh"

using namespace jeod;

/*****************************************************************************
Constructors
******************************************************************************/
LsodeIntegrationControls::LsodeIntegrationControls (void)
{}

LsodeIntegrationControls::LsodeIntegrationControls (
   unsigned int num_stages JEOD_UNUSED)
{}


/*****************************************************************************
create_copy
******************************************************************************/
LsodeIntegrationControls * LsodeIntegrationControls::create_copy()
const
{
   er7_utils::MessageHandler::fail (__FILE__, __LINE__,
      er7_utils::IntegrationMessages::internal_error,
      "Copy constructor for LsodeIntegrationControls "
      "not implemented.\n");
   return NULL;
}

/**
 * Perform one step of the integration process.
 */
unsigned int
LsodeIntegrationControls::integrate (
   double starttime,
   double sim_dt,
   er7_utils::TimeInterface & time_interface,
   er7_utils::IntegratorInterface & integ_interface,
   er7_utils::BaseIntegrationGroup & integ_group)
{

   // Starting a new integration tour needs special processing.
   if (step_number == 0) {

      // Reset the integrators, time if the meaning of time has changed.
      if (reset_needed || (!Numerical::compare_exact(integ_simdt,sim_dt))) {

         // Reset integrators.
         integ_group.reset_body_integrators ();
//         reset_integrator ();

         // Update timing information.
         time_scale_factor = time_interface.get_time_scale_factor();
         integ_simdt = sim_dt;
         integ_dyndt = sim_dt * time_scale_factor;

         // Mark the reset as handled.
         reset_needed = false;
         integ_interface.restore_first_step_derivs_flag ();
      }

      // Cache the integration tour start time.
      integ_starttime = starttime;

      cycle_starttime  = integ_starttime;
      cycle_simdt      = integ_simdt;
      cycle_dyndt      = integ_dyndt;
      integ_simtime    = integ_starttime;
      integ_time_scale = 0.0;
   }


   const er7_utils::IntegratorResult & integ_status =
      integ_group.integrate_bodies (integ_dyndt, 1);
   bool target_attained = integ_status.get_passed();
   double time_scale = integ_status.get_time_scale();

   // Advance time to the next stage.
   if (!Numerical::compare_exact(integ_time_scale,time_scale)) {
      integ_time_scale = time_scale;
      integ_simtime    = cycle_starttime + time_scale*cycle_simdt;
      time_interface.update_time (integ_simtime);
   }

   // Target attained by all integrators:
   // Advance the cycle stage to the attained target stage.
   if (target_attained) {
      step_number = 0;
   }
   else {
      step_number++;
   }

   // Update the sim engine's integration interface structure.
   integ_group.update_integration_interface (step_number);

   return step_number;
}

/**
 * @}
 * @}
 * @}
 * @}
 */
