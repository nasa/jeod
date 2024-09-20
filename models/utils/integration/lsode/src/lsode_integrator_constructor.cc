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
 * @file models/utils/integration/lsode/src/lsode_integrator_constructor.cc
 * Define the methods in the class LsodeIntegratorConstructor.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  (())

Library dependencies:
  ((lsode_integrator_constructor.cc)
   (lsode_integration_controls.cc))



*******************************************************************************/

// System includes

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// Integration includes
#include "er7_utils/integration/core/include/integration_messages.hh"
#include "er7_utils/integration/core/include/integrator_constructor_utils.hh"
#include "er7_utils/interface/include/message_handler.hh"

// Model includes
#include "../include/lsode_first_order_ode_integrator.hh"
#include "../include/lsode_generalized_second_order_ode_integrator.hh"
#include "../include/lsode_integration_controls.hh"
#include "../include/lsode_integrator_constructor.hh"
#include "../include/lsode_second_order_ode_integrator.hh"
#include "../include/lsode_simple_second_order_ode_integrator.hh"

using namespace jeod;

/*****************************************************************************
copy constructor
******************************************************************************/
LsodeIntegratorConstructor::LsodeIntegratorConstructor(const LsodeIntegratorConstructor & src)
    : er7_utils::IntegratorConstructor(),
      data_interface(src.data_interface)
{
}

// Named constructor; create an LsodeIntegratorConstructor.
er7_utils::IntegratorConstructor * LsodeIntegratorConstructor::create_constructor()
{
    return er7_utils::alloc::allocate_object<LsodeIntegratorConstructor>();
}

// Create a duplicate of the constructor.
er7_utils::IntegratorConstructor * LsodeIntegratorConstructor::create_copy() const
{
    return er7_utils::alloc::replicate_object(*this);
}

// Create an integration controls for Lsode.
er7_utils::IntegrationControls * LsodeIntegratorConstructor::create_integration_controls() const
{
    return er7_utils::alloc::allocate_object<LsodeIntegrationControls>();
}

// Create an Lsode integrator for a first order ODE.
er7_utils::FirstOrderODEIntegrator * LsodeIntegratorConstructor::create_first_order_ode_integrator(
    unsigned int size, er7_utils::IntegrationControls & controls) const
{
    return er7_utils::alloc::allocate_object<LsodeFirstOrderODEIntegrator,
                                             const LsodeControlDataInterface &,
                                             er7_utils::IntegrationControls &,
                                             unsigned int>(data_interface, controls, size);
}

// Create an RK4 state integrator for a second order ODE.
er7_utils::SecondOrderODEIntegrator * LsodeIntegratorConstructor::create_second_order_ode_integrator(
    unsigned int size, er7_utils::IntegrationControls & controls) const
{
    return er7_utils::alloc::allocate_object<LsodeSimpleSecondOrderODEIntegrator,
                                             const LsodeControlDataInterface &,
                                             er7_utils::IntegrationControls &,
                                             unsigned int>(data_interface, controls, size);
}

// Create an Lsode state integrator for a second order ODE.
er7_utils::SecondOrderODEIntegrator * LsodeIntegratorConstructor::create_generalized_deriv_second_order_ode_integrator(
    unsigned int position_size,
    unsigned int velocity_size,
    const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
    er7_utils::IntegrationControls & controls) const
{
    auto & non_const_data_interface = const_cast<LsodeControlDataInterface &>(data_interface);

    return er7_utils::alloc::allocate_object<LsodeGeneralizedDerivSecondOrderODEIntegrator,
                                             LsodeControlDataInterface &,
                                             er7_utils::IntegrationControls &,
                                             const er7_utils::GeneralizedPositionDerivativeFunctions &,
                                             unsigned int,
                                             unsigned int>(non_const_data_interface,
                                                           controls,
                                                           deriv_funs,
                                                           position_size,
                                                           velocity_size);
}

/**
 * @}
 * @}
 * @}
 * @}
 */
