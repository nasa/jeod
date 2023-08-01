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
 * @file models/utils/integration/lsode/src/lsode_simple_second_order_ode_integrator.cc
 * Define member functions for the class LsodeSimpleSecondOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((lsode_simple_second_order_ode_integrator.cc))

 

*******************************************************************************/


// System includes

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// Integration includes
#include "er7_utils/interface/include/message_handler.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"

// Model includes
#include "../include/lsode_control_data_interface.hh"
#include "../include/lsode_simple_second_order_ode_integrator.hh"

using namespace jeod;

// Default constructor
LsodeSimpleSecondOrderODEIntegrator::LsodeSimpleSecondOrderODEIntegrator(void)
:
   LsodeSecondOrderODEIntegrator()
{}


// Non-default, basic constructor
LsodeSimpleSecondOrderODEIntegrator::LsodeSimpleSecondOrderODEIntegrator(
              const LsodeControlDataInterface & data_in,
              er7_utils::IntegrationControls & controls,
              unsigned int size)
:
   LsodeSecondOrderODEIntegrator(data_in, controls, size)
{}



// Clone a LsodeSimpleSecondOrderODEIntegrator, which is illegal.
LsodeSimpleSecondOrderODEIntegrator *
LsodeSimpleSecondOrderODEIntegrator::create_copy ()
const
{
   er7_utils::MessageHandler::fail (__FILE__, __LINE__,
      er7_utils::IntegrationMessages::internal_error,
      "Copy constructor for LsodeSecondOrderODEIntegrator not implemented.\n");
   return nullptr;
}



// Propagate state using LSODE.
er7_utils::IntegratorResult
LsodeSimpleSecondOrderODEIntegrator::integrate (
   double dyn_dt,
   unsigned int target_stage JEOD_UNUSED,
   double const * ER7_UTILS_RESTRICT accel,
   double * ER7_UTILS_RESTRICT velocity,
   double * ER7_UTILS_RESTRICT position)
{
   er7_utils::IntegratorResult return_val;  // Don't move this line.  First entry for optimization


   for (unsigned int ii = 0; ii < zeroth_derivative_size; ii++) {
      y[ii] = position[ii];
      y[ii+zeroth_derivative_size] = velocity[ii];
      y_dot[ii] = velocity[ii];
      y_dot[ii+zeroth_derivative_size] = accel[ii];
   }
   return_val =  first_order_integrator.integrate( dyn_dt,
                                                    1,
                                                    y_dot,
                                                    y);
   for (unsigned int ii = 0; ii < zeroth_derivative_size; ii++) {
      position[ii] = y[ii];
      velocity[ii] = y[ii+zeroth_derivative_size];
   }

   return return_val;
}

/**
 * @}
 * @}
 * @}
 * @}
 */
