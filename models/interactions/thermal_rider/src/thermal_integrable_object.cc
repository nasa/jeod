/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup ThermalRider
 * @{
 *
 * @file models/interactions/thermal_rider/src/thermal_integrable_object.cc
 * Encapsulates an integrator for a single facet
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((thermal_integrable_object.cc)
 (thermal_facet_rider.cc))


*******************************************************************************/

/* System includes */
#include <cmath>

/*  JEOD includes */

/* Model structure includes */
#include "../include/thermal_facet_rider.hh"
#include "../include/thermal_integrable_object.hh"
#include "../include/thermal_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * ThermalIntegrableObject default constructor.
 */
ThermalIntegrableObject::ThermalIntegrableObject()
{
    JEOD_REGISTER_CLASS(ThermalIntegrableObject);
    JEOD_REGISTER_CHECKPOINTABLE(this, integrator);
}

/**
 * ThermalIntegrableObject destructor.
 */
ThermalIntegrableObject::~ThermalIntegrableObject()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, integrator);
    destroy_integrators();
}

/**
 * Create the first order integrator for this IntegrableObject.
 * @param generator  Integrator constructor that creates the integrator.
 * @param controls   Integration controls that mediates the integrations.
 * @param time_if    Unused.
 */
void ThermalIntegrableObject::create_integrators(const er7_utils::IntegratorConstructor & generator,
                                                 er7_utils::IntegrationControls & controls,
                                                 const er7_utils::TimeInterface & time_if JEOD_UNUSED)
{
    integrator.create_integrator(generator, controls);
}

/**
 * Destroy integrators for this IntegrableObject.
 */
void ThermalIntegrableObject::destroy_integrators()
{
    // nothing to do
}

/**
 * Reset the integrator.
 */
void ThermalIntegrableObject::reset_integrators()
{
    integrator.reset_integrator();
}

/**
 * Integrate the thermal state
 * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
 * @param[in]     target_stage  The stage of the integration process
 *                              that the integrator should try to attain.
 * @return The status (time advance, pass/fail status) of the integration.
 */
er7_utils::IntegratorResult ThermalIntegrableObject::integrate(double dyn_dt, unsigned int target_stage)
{
    double t_eq_pow4 = rider->power_absorb / rider->rad_constant;
    er7_utils::IntegratorResult result = integrator.integrate(dyn_dt, target_stage, &temp_dot, &temp);

    t_pow4 = temp * temp;
    t_pow4 *= t_pow4;

    if(temp_dot * (t_eq_pow4 - t_pow4) < 0.0)
    { // overshot the asymtotic value
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               ThermalMessages::invalid_integration_operation,
                               "\n"
                               "Temperature integration produced instability.\n"
                               "The integration step is sufficiently large that the predicted\n"
                               "temperature at  stage %u of the integration is on the \n"
                               "wrong side of the asymptote.  \n"
                               "The predicted temperature is being set to the equilibrium"
                               "temperature.\n",
                               target_stage);
        t_pow4 = t_eq_pow4;
        temp = std::sqrt(std::sqrt(t_pow4));
    }

    return result;
}

/**
 * Initialize temperature and cache a pointer to the ThermalFacetRider
 * @param temperature -- initial temperature
 * @param associated_rider -- the associated ThermalFacetRider
 */
void ThermalIntegrableObject::initialize(double temperature, ThermalFacetRider & associated_rider)
{
    temp = temperature;
    rider = &associated_rider;
    t_pow4 = temp * temp;
    t_pow4 *= t_pow4;
}

/**
 * Compute emitted power and the time derivative of temperature.
 */
void ThermalIntegrableObject::compute_temp_dot()
{
    rider->power_emit = rider->rad_constant * t_pow4;
    temp_dot = (rider->power_absorb - rider->power_emit) / rider->heat_capacity;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
