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
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_config.cc
 * Defines member functions for the class GaussJacksonIntegratorConstructor.
 */

/*
Purpose: ()
*/

/* JEOD includes */
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/gauss_jackson_config.hh"

// Integration includes
#include "er7_utils/integration/core/include/integration_messages.hh"

// Interface includes
#include "er7_utils/interface/include/message_handler.hh"

// System inclludes
#include <algorithm>
#include <string>

//! Namespace jeod
namespace jeod
{

GaussJacksonConfig GaussJacksonConfig::default_configuration()
{
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    GaussJacksonConfig result{};

    result.priming_technique = er7_utils::Integration::Unspecified;
    result.initial_order = -1;
    result.final_order = -1;
    result.ndoubling_steps = -1;
    result.relative_tolerance = -1;
    result.absolute_tolerance = -1;
    result.max_correction_iterations = -1;

    return result;
}

GaussJacksonConfig GaussJacksonConfig::standard_configuration()
{
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    GaussJacksonConfig result{};

    result.priming_technique = er7_utils::Integration::RKFehlberg78;
    result.initial_order = 8;
    result.final_order = 12;
    result.ndoubling_steps = 2;
    result.relative_tolerance = 1e-14;
    result.absolute_tolerance = 1e-14;
    result.max_correction_iterations = 10;

    return result;
}

/**
 * Swap the negative ones in the supplied config with the default values,
 * some of which are computed.
 */
static GaussJacksonConfig set_default_config_values(const GaussJacksonConfig & config)
{
    GaussJacksonConfig result = config;

    // Set the initial order if it is at the default.
    if(result.initial_order == -1u)
    {
        // The initial order must not exceed the final order, so a final order
        // of 2 dictates that the initial order be 2.
        if(result.final_order == 2)
        {
            result.initial_order = 2;
        }

        // The initial order is set to 8 if this is consistent with the final
        // order and if this is compatible with the priming technique.
        else if((result.final_order >= 8) && ((result.priming_technique == er7_utils::Integration::Unspecified) ||
                                              (result.priming_technique == er7_utils::Integration::RKFehlberg78)))
        {
            result.initial_order = 8;
        }

        // Use 4 if neither of the above cases applies.
        else
        {
            result.initial_order = 4;
        }
    }

    // Set the final order if it is at the default.
    // Use 14 if the initial order is 14, 12 otherwise.
    if(result.final_order == -1u)
    {
        if(result.initial_order == 14)
        {
            result.final_order = 14;
        }
        else
        {
            result.final_order = 12;
        }
    }

    // Set the priming technique if it is at the default.
    // Use RK2 if the initial order is 2, RKFehlberg78 if the initial order is
    // 8 or more, RK4 otherwise.
    if(result.priming_technique == er7_utils::Integration::Unspecified)
    {
        if(result.initial_order == 2)
        {
            result.priming_technique = er7_utils::Integration::RK2Midpoint;
        }
        else if(result.initial_order >= 8)
        {
            result.priming_technique = er7_utils::Integration::RKFehlberg78;
        }
        else
        {
            result.priming_technique = er7_utils::Integration::RungeKutta4;
        }
    }

    // Set the number of doubling steps if it is at the default,
    // but only if it makes sense to do so.
    if((result.ndoubling_steps == -1u) && (result.final_order >= result.initial_order) && (result.final_order <= 14))
    {
        result.ndoubling_steps = std::min(1u, (result.final_order - result.initial_order) / 2u);
    }

    // Set the maximum corrections allowed if it is at the default.
    if(result.max_correction_iterations == -1u)
    {
        result.max_correction_iterations = 10;
    }

    // Set the tolerances if they are at the defaults.
    if(Numerical::compare_exact(result.relative_tolerance, -1.0))
    {
        result.relative_tolerance = 1e-14;
    }

    if(Numerical::compare_exact(result.absolute_tolerance, -1.0))
    {
        result.absolute_tolerance = 1e-10;
    }

    return result;
}

/**
 * Check for invalid values in the supplied config.
 */
static unsigned int validate_config(const GaussJacksonConfig & config)
{
    unsigned int error_count = 0;

    // The initial order must be an even number between 2 and 14.
    if(((config.initial_order & 1) != 0) || (config.initial_order < 2) || (config.initial_order > 14))
    {
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "Illegal Gauss-Jackson configuration:\n"
                                         "The initial order %u must be an even integer between 2 and 14.",
                                         config.initial_order);
        ++error_count;
    }

    // The final order must be an even number between initial_order and 14.
    if(((config.final_order & 1) != 0) || (config.final_order < config.initial_order) || (config.final_order > 14))
    {
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "Illegal Gauss-Jackson configuration:\n"
                                         "The final order %u must be an even integer between\n"
                                         "The initial order %u and 14.",
                                         config.final_order,
                                         config.initial_order);
        ++error_count;
    }

    // The number of doubling steps must be 20 or less.
    if(config.ndoubling_steps > 20)
    {
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "Illegal Gauss-Jackson configuration:\n"
                                         "The number of doubling steps %u is not 20 or less.",
                                         config.ndoubling_steps);
        ++error_count;
    }

    // The relative tolerance must be between 0 and 1.
    if((config.relative_tolerance < 0.0) || (config.relative_tolerance > 1.0))
    {
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "Illegal Gauss-Jackson configuration:\n"
                                         "The relative tolerance %g must be between 0 and 1, inclusive.",
                                         config.relative_tolerance);
        ++error_count;
    }

    // The absolute tolerance must be non-negative.
    if(config.absolute_tolerance < 0.0)
    {
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "Illegal Gauss-Jackson configuration:\n"
                                         "The absolute tolerance %g must be non-negative.",
                                         config.relative_tolerance);
        ++error_count;
    }

    return error_count;
}

GaussJacksonConfig GaussJacksonConfig::validate_configuration(const GaussJacksonConfig & config)
{
    // Set defaults.
    GaussJacksonConfig result = set_default_config_values(config);

    // Check for violations.
    unsigned int error_count = validate_config(result);

    // Any violations means to reject the input and use the standard instead.
    if(error_count > 0)
    {
        const char errorStrings[2][7] = {"errors", "error"};
        int index = 1;
        if(error_count > 1)
        {
            index = 0;
        }
        er7_utils::MessageHandler::error(__FILE__,
                                         __LINE__,
                                         er7_utils::IntegrationMessages::invalid_request,
                                         "The default Gauss-Jackson configuration "
                                         "will be used instead of the supplied configuration\n"
                                         "because of the above %s.",
                                         errorStrings[index]);
        result = standard_configuration();
    }

    return result;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
