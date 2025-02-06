/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup GaussJackson
 * @{
 *
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_generalized_second_order_ode_integrator.cc
 * Defines member functions for the class
 * GaussJacksonGeneralizedDerivSecondOrderODEIntegrator.
 */

/*
Purpose: ()
*/

// Local includes
#include "../include/gauss_jackson_generalized_second_order_ode_integrator.hh"

// ER7 utilities includes
#include "er7_utils/interface/include/alloc.hh"

// System includes
#include <algorithm>

//! Namespace jeod
namespace jeod
{

// Non-default constructor.
GaussJacksonGeneralizedDerivSecondOrderODEIntegrator::GaussJacksonGeneralizedDerivSecondOrderODEIntegrator(
    const er7_utils::IntegratorConstructor & primer_constructor,
    GaussJacksonIntegrationControls & controls,
    unsigned int position_size,
    unsigned int velocity_size,
    const er7_utils::GeneralizedPositionDerivativeFunctions & deriv_funs,
    er7_utils::IntegrationControls & priming_controls)
    : SecondOrderODEIntegrator(position_size, velocity_size, deriv_funs, controls),
      vel_integrator(primer_constructor, controls, velocity_size, priming_controls),
      pos_integrator(primer_constructor, controls, position_size, priming_controls)
{
    posdot = er7_utils::alloc::allocate_array<double>(position_size);
    posdotdot = er7_utils::alloc::allocate_array<double>(position_size);
}

// Copy constructor.
GaussJacksonGeneralizedDerivSecondOrderODEIntegrator::GaussJacksonGeneralizedDerivSecondOrderODEIntegrator(
    const GaussJacksonGeneralizedDerivSecondOrderODEIntegrator & src)
    : SecondOrderODEIntegrator(src),
      vel_integrator(src.vel_integrator),
      pos_integrator(src.pos_integrator)
{
    posdot = er7_utils::alloc::replicate_array<double>(state_size[0], src.posdot);
    posdotdot = er7_utils::alloc::replicate_array<double>(state_size[0], src.posdotdot);
}

// Destructor.
GaussJacksonGeneralizedDerivSecondOrderODEIntegrator::~GaussJacksonGeneralizedDerivSecondOrderODEIntegrator()
{
    er7_utils::alloc::deallocate_array<double>(posdot);
    er7_utils::alloc::deallocate_array<double>(posdotdot);
}

// Non-throwing swap.
void GaussJacksonGeneralizedDerivSecondOrderODEIntegrator::swap(
    GaussJacksonGeneralizedDerivSecondOrderODEIntegrator & other)
{
    SecondOrderODEIntegrator::swap(other);

    vel_integrator.swap(other.vel_integrator);
    pos_integrator.swap(other.pos_integrator);

    std::swap(posdot, other.posdot);
    std::swap(posdotdot, other.posdotdot);
}

// Replicate this.
er7_utils::SecondOrderODEIntegrator * GaussJacksonGeneralizedDerivSecondOrderODEIntegrator::create_copy() const
{
    return er7_utils::alloc::replicate_object(*this);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
