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
 * @file models/utils/integration/lsode/src/lsode_second_order_ode_integrator.cc
 * Define member functions for the class LsodeSecondOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (TBS)

Library dependencies:
  ((lsode_second_order_ode_integrator.cc))

 

*******************************************************************************/


// System includes

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// Integration includes
#include "er7_utils/integration/core/include/integ_utils.hh"
#include "er7_utils/interface/include/message_handler.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"
#include "er7_utils/integration/core/include/integration_controls.hh"

// Model includes
#include "../include/lsode_control_data_interface.hh"
#include "../include/lsode_second_order_ode_integrator.hh"

using namespace jeod;

// LsodeSecondOrderODEIntegrator default constructor.
LsodeSecondOrderODEIntegrator::LsodeSecondOrderODEIntegrator (
   void)
:
   Er7UtilsDeletable (),
   SecondOrderODEIntegrator (),
   y(NULL),
   y_dot(NULL),
   zeroth_derivative_size(0),
   first_derivative_size(0),
   arrays_allocated(false)
{ }



// LsodeSecondOrderODEIntegrator non-default constructor for a simple
// second order ODE.
LsodeSecondOrderODEIntegrator::LsodeSecondOrderODEIntegrator (
   const LsodeControlDataInterface & data_in,
   er7_utils::IntegrationControls & controls,
   unsigned int size)
:
   er7_utils::Er7UtilsDeletable (),
   er7_utils::SecondOrderODEIntegrator (size, controls),
   y(nullptr),
   y_dot(nullptr),
   zeroth_derivative_size(size),
   first_derivative_size(size),
   first_order_integrator(data_in, controls, 2*size),
   arrays_allocated(false)
{
   y = er7_utils::alloc::allocate_array<double> (2*size);
   y_dot = er7_utils::alloc::allocate_array<double> (2*size);
   arrays_allocated = true;
}

// LsodeSecondOrderODEIntegrator non-default constructor for a generalized
// second order ODE in which position is advanced internally using the position
// derivative computed by the provided derivative function.
LsodeSecondOrderODEIntegrator::LsodeSecondOrderODEIntegrator (
   const LsodeControlDataInterface & data_in,
   er7_utils::IntegrationControls & controls,
   const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
   unsigned int position_size,
   unsigned int velocity_size)
:
   Er7UtilsDeletable (),
   SecondOrderODEIntegrator (position_size, velocity_size,
                             deriv_funs, controls),
   y(nullptr),
   y_dot(nullptr),
   zeroth_derivative_size(position_size),
   first_derivative_size(velocity_size),
   first_order_integrator(data_in, controls, position_size+velocity_size),
   arrays_allocated(false)

{
   // Allocate memory used by Lsode integration.
   y = er7_utils::alloc::allocate_array<double> (position_size+velocity_size);
   y_dot = er7_utils::alloc::allocate_array<double> (position_size+velocity_size);
   arrays_allocated = true;
}




// LsodeSecondOrderODEIntegrator destructor.
LsodeSecondOrderODEIntegrator::~LsodeSecondOrderODEIntegrator (
   void)
{
   if (arrays_allocated) {
      er7_utils::alloc::deallocate_array<double> (y);
      er7_utils::alloc::deallocate_array<double> (y_dot);
   }
}

/**
 * @}
 * @}
 * @}
 * @}
 */
