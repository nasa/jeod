/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_base_facet.cc
 * Define member functions for class RadiationBaseFacet
 */

/*****************************************************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
    ((RadiationBaseFacet is non-instantiable))

LIBRARY DEPENDENCY:
    ((radiation_base_facet.cc)
     (radiation_messages.cc)
     (utils/message/src/message_handler.cc))



******************************************************************************/

// System includes

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/radiation_base_facet.hh"
#include "../include/radiation_messages.hh"
#include "../include/radiation_third_body.hh"

//! Namespace jeod
namespace jeod
{

/**
 * initializes the base surface
 */
void RadiationBaseFacet::initialize()
{
    // albedo and albedo_vis are synonymous at initialization.  During runtime,
    // albedo can take the value albedo_vis or albedo_IR, but legacy code had
    // only one albedo (repesenting albedo_vis), called albedo.
    bool setequ = false;
    if(albedo < 0.0)
    {
        albedo = albedo_vis;
        setequ = true;
    }
    if(albedo_vis < 0.0)
    {
        albedo_vis = albedo;
        setequ = true;
    }
    // If albedo_IR not set, assume equal to albedo.
    if(albedo_IR < 0.0)
    {
        albedo_IR = albedo;
    }

    if(!setequ)
    { // So both albedo and albedo_vis were set to legal values.
        double epsilon = 1.0E-12;
        if((albedo - albedo_vis) > epsilon || (albedo - albedo_vis) < -epsilon)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 RadiationMessages::invalid_setup_error,
                                 "\n"
                                 "The values albedo and albedo_vis are synonymous at initialization.\n"
                                 "However, they have been set to different values (%f and %f"
                                 "respectively)\n.  Unsure how to interpret these data.\n",
                                 albedo,
                                 albedo_vis);
        }
    }
}

/**
 * Calculation of force, torques and irradiance due to ThirdBody flux.
 * \param[in,out] third_body_ptr Third body that emits radiation
 * \param[in] calculate_forces Calculate forces on this facet if true.
 */
void RadiationBaseFacet::interact_with_third_body(RadiationThirdBody * third_body_ptr, const bool calculate_forces)
{
    // Reflective flux:
    albedo = albedo_vis;
    third_body_ptr->accumulate_refl_flux(this, calculate_forces);

    // Radiative flux (i.e. Infra-red)
    albedo = albedo_IR;
    third_body_ptr->accumulate_rad_flux(this, calculate_forces);
    albedo = albedo_vis;
}

/**
 * To initialize the values during each update run
 */
void RadiationBaseFacet::initialize_runtime_values()
{
    thermal.power_absorb = 0.0;
    Vector3::initialize(F_diffuse);
    Vector3::initialize(F_absorption);
    Vector3::initialize(F_specular);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
