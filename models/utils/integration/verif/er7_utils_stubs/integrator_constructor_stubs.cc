/*
 * integrator_constructor_stubs.cc
 *
 *  Created on: Jan 15, 2024
 *      Author: tbrain
 */

#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "utils/sim_interface/include/config.hh"

namespace er7_utils
{

IntegratorResultMerger * IntegratorConstructor::create_integrator_results_merger() const
{
    return nullptr;
}

FirstOrderODEIntegrator * IntegratorConstructor::create_first_order_ode_integrator(unsigned int,
                                                                                   IntegrationControls &) const
{
    return nullptr;
}

SecondOrderODEIntegrator * IntegratorConstructor::create_second_order_ode_integrator(unsigned int,
                                                                                     IntegrationControls &) const
{
    return nullptr;
}

SecondOrderODEIntegrator * IntegratorConstructor::create_generalized_deriv_second_order_ode_integrator(
    unsigned int, unsigned int, const GeneralizedPositionDerivativeFunctions &, IntegrationControls &) const
{
    return nullptr;
}

SecondOrderODEIntegrator * IntegratorConstructor::create_generalized_step_second_order_ode_integrator(
    unsigned int, unsigned int, const GeneralizedPositionStepFunctions &, IntegrationControls &) const
{
    return nullptr;
}

} // namespace er7_utils
