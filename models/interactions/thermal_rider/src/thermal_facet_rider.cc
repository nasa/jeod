/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup ThermalRider
 * @{
 *
 * @file models/interactions/thermal_rider/src/thermal_facet_rider.cc
 * ThermalFacetRider interaction model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((The thermal rider works with a surface interaction model.  By adding a
      ThermalFacetRiderRider object to the InteractionFacet, the temperature can be
      monitored and integrated.)
      (This is not a stand-alone interaction model))

Library dependencies:
   ((thermal_facet_rider.cc)
    (thermal_integrable_object.cc)
    (thermal_messages.cc)
    (utils/message/src/message_handler.cc))


*******************************************************************************/

/* System includes */
#include <cmath>
#include <cstddef>

/*  JEOD includes */
#include "utils/message/include/message_handler.hh"

/* Model structure includes */
#include "../include/thermal_facet_rider.hh"
#include "../include/thermal_messages.hh"

//! Namespace jeod
namespace jeod
{

// Static default values.

const double ThermalFacetRider::stefan_boltzmann = 5.6704004E-08;

double ThermalFacetRider::cycle_time = 0.0;

/**
 * Collects together all surface and internal thermal sources that affect
 * a facet.
 */
void ThermalFacetRider::accumulate_thermal_sources()
{
    power_absorb += thermal_power_dump;

    // For future implementation: Facet-to-Facet conduction
    // Need the following:
    //   1. a model specific conduction_active flag (i.e. one for aero, one for
    //      radiation, etc.)
    //   2. a model-specific conduction_enabled flag (determines whether to
    //      initialize the variables listed below)
    //   3. an array of pointers to those interaction facets (in the same interaction
    //      surface) to which conduction is possible.
    //   4. a conductivity constant between this interaction facet and all others
    //      in the previous array
    // Implement by:
    //  1 & 2 Inheriting from ThermalFacetRider, say a RadiationThermalFacetRider, with a
    //        static bool conduction_active that all Radiation facets would then
    //        see.
    //  3 & 4 Have to be done after the Interaction Surface is built, b/c the
    //        pointers have to be to InteractionFacets, not to Facets.  Hence,
    //        knowledge of where each InteractionFacet is in the
    //        InteractionSurface will be needed.  Where there are multiple
    //        topologies, this is non-trivial, hence the non-implementation.
    // The code:
    // if ( conduction_active && conduction_enabled ) {
    //  for (int ii_facet = 0 ; ii_facet < num_contact_facets; ++ii_facet) {
    //    if ( facet_ptr[ii_facet] != nullptr) {
    //      cond_calc = conduction[ii_facet] *
    //                 (facet_ptr[ii_facet]->thermal.temperature - temperature);
    //      power_absorb += cond_calc;
    //      facet_ptr[ii_facet]->thermal.power_absorb -= cond_calc;
    //    }
    //  }
    // }

    // For future inclusion:  Aerodynamic heating.
    // Add value based on aerodynamic drag and velocity.

    //  Set value for power_emit.  If the temperature is not integrated, this will
    //  be true.  If the temperature is integrated, this will be overwritten.
    power_emit = power_absorb;
}

/**
 * Initialize the thermal aspects of the facet.
 * \param[in] temperature temperature of facet\n Units: K
 * \param[in] surface_area area of facet surface\n Units: M*M
 */
void ThermalFacetRider::initialize(double temperature, double surface_area)
{
    next_temperature = temperature;
    dynamic_temperature = temperature;
    if(emissivity < 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             ThermalMessages::incomplete_setup_error,
                             "\n"
                             "Emissivity was set to an invalid number. Emissivity must be > 0.\n");
    }

    if(emissivity < 1.0E-12)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             ThermalMessages::incomplete_setup_error,
                             "\n"
                             "Emissivity was set to too small a value.  Resetting emissivity to "
                             "1.0E-12.\n");
        emissivity = 1.0E-12;
    }

    if(surface_area <= 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             ThermalMessages::incomplete_setup_error,
                             "\n"
                             "Surface Area was set to an invalid number. Area must be > 0.\n");
    }
    if(surface_area < 1.0E-12)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             ThermalMessages::incomplete_setup_error,
                             "\n"
                             "Surface Area was set to too small a value.  Resetting surface_area to "
                             "1.0E-12 m^2.\n");
        surface_area = 1.0E-12;
    }

    rad_constant = surface_area * emissivity * stefan_boltzmann;

    integrable_object.initialize(temperature, *this);
}

/**
 * Runge-Kutta 4th order integration of the temperature variation
 * @return void
 */
double ThermalFacetRider::integrate()
{
    if(!active)
    {
        power_emit = power_absorb; // Keep the physics consistent!
        return dynamic_temperature;
    }

#define pow4(x) pow4_temp = (x) * (x), pow4_temp *= pow4_temp

    double pow4_temp; /* -- temporary value associated with T^4 macro, pow4     */
    double T0;        /* -- starting temperature */
    double I1;        /* -- intermediate value   */
    double cyc_cap = cycle_time / heat_capacity;
    /* -- shorthand collection of values used in emission calcs*/
    double Teq4; /*  K*K*K*K
               Equilibrium temperature to 4th power.*/
    int dt_dir;  /* --
               direction in which temperature should go: +1 up, -1 down. */

    //  assign temperature from previously calculated value.
    dynamic_temperature = next_temperature;

    /* R-K 4 integration of energy flux to obtain new temperature         */
    T0 = dynamic_temperature;
    Teq4 = power_absorb / rad_constant;

    pow4(T0);
    if(pow4_temp > Teq4)
    { // above equilibrium
        dt_dir = -1;
    }
    else
    {
        dt_dir = 1;
    }

    I1 = cyc_cap * (power_absorb - rad_constant * pow4_temp);
    pow4(T0 + I1 / 2);

    double I2; /* intermediate value   */
    I2 = cyc_cap * (power_absorb - rad_constant * pow4_temp);

    if(I2 * dt_dir < 0)
    { // jumped through asymptote.
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               ThermalMessages::invalid_integration_operation,
                               "\n"
                               "Temperature integration produced instability.\n"
                               "The integration step is sufficiently large that the predicted\n"
                               "temperature at the first stage of the integration is on the \n"
                               "wrong side of the asymptote.  \n"
                               "The predicted temperature is being set to the equilibrium"
                               "temperature.\n");
        next_temperature = sqrt(sqrt(Teq4));
        d_temperature = next_temperature - dynamic_temperature;
        power_emit = power_absorb - (heat_capacity * d_temperature / cycle_time);
        return dynamic_temperature;
    }

    double I3; /* intermediate value   */
    pow4(T0 + I2 / 2);
    I3 = cyc_cap * (power_absorb - rad_constant * pow4_temp);

    double I4; /* intermediate value   */
    pow4(T0 + I3);
    I4 = cyc_cap * (power_absorb - rad_constant * pow4_temp);

    if(dt_dir * I4 < 0)
    {
        I4 *= 0.5;
    }

    d_temperature = (I1 + 2 * I2 + 2 * I3 + I4) / 6;

    if((d_temperature < -dynamic_temperature) || (d_temperature > 1E6))
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             ThermalMessages::invalid_integration_operation,
                             "\n"
                             "Temperature integration gone awry for unknown reason.\n"
                             "Resetting flag on facet and holding temperature constant.\n");
        active = false;
        return dynamic_temperature;
    }

    if(dt_dir * d_temperature < 0)
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               ThermalMessages::invalid_integration_operation,
                               "\n"
                               "Temperature integration produced instability.\n"
                               "The integration step is sufficiently large that the predicted\n"
                               "value for temperature went through the asymptote; this led to\n"
                               "an invalid temperature prediction, that is farther from equilibrium \n"
                               "than when it started.  Setting the predicted temperature to the \n"
                               "equilibrium temperature.");
        next_temperature = sqrt(sqrt(Teq4));
        d_temperature = next_temperature - dynamic_temperature;
    }
    else
    { // temperature went right direction, but did it go too far?

        next_temperature = dynamic_temperature + d_temperature;
        pow4(next_temperature);

        if(dt_dir * (Teq4 - pow4_temp) < 0)
        { // overshot equilibrium value
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   ThermalMessages::invalid_integration_operation,
                                   "\n"
                                   "Temperature integration overshot target temperature.\n"
                                   "Probably due to an excessively large integration step,\n"
                                   "the temperature integrator produced a temperature on the wrong \n"
                                   "side of the equilibrium temperature. \n"
                                   "Setting the predicted temperature to the equilibrium temperature.");
            next_temperature = sqrt(sqrt(Teq4));
            d_temperature = next_temperature - dynamic_temperature;
        }
    }

    power_emit = power_absorb - (heat_capacity * d_temperature / cycle_time);

    return dynamic_temperature;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
