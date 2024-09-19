/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/aero_drag.cc
 * Orbital aerodynamic force and torque computation, and related
 * classes.
 */

/*
PURPOSE:
    ()


ASSUMPTIONS AND LIMITATIONS:
    ((Orbital body modelled as a series of geometric plates, or as
      a simple ballistic coefficient or coefficient of drag, unless
      the user has overriden any of these behaviors)
     (All plate information referenced to the orbital vehicle's structural
      frame.)
     (No plate oclusion)
     (Only "wind-ward" facing plates, defined via the "normal" parameter,
      are used for force/torque calculations)
     (Designed for use at an altitude where the atmosphere is a free molecular
      flow regime -- orbital applications.)
     (Atmospheric altitude and density are calculated in an external
      "environment" module)
     (Output forces and torques are referenced to the vehicle structural frame)
     (Input cg location is relative and referenced to the structural frame))


LIBRARY DEPENDENCY:
    ((aerodynamics_messages.cc)
     (aero_surface.cc)
     (default_aero.cc)
     (utils/message/src/message_handler.cc))


*/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "environment/atmosphere/base_atmos/include/atmosphere.hh"
#include "environment/atmosphere/base_atmos/include/atmosphere_state.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/aero_drag.hh"
#include "../include/aero_facet.hh"
#include "../include/aero_surface.hh"
#include "../include/aerodynamics_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * default constructor
 * Return: -- void
 */
AerodynamicDrag::AerodynamicDrag()
    : default_behavior(&ballistic_drag)
{
}

/**
 * Calculates the total aerodynamic drag force and torque, from
 * the information given
 * \param[in] inertial_velocity vehicle velocity in inertial RF\n Units: M/s
 * \param[in] atmos_ptr Pointer to the AtmosphereState used for density and wind information
 * \param[in] T_inertial_struct Transformation matrix from the inertial frame to the structural
 * \param[in] mass kg Mass of the vehicle
 * \param[in] center_grav position of the center of gravity, in the structural frame\n Units: M
 */

void AerodynamicDrag::aero_drag(double inertial_velocity[3],
                                AtmosphereState * atmos_ptr,
                                double T_inertial_struct[3][3],
                                double mass,
                                double center_grav[3])
{
    unsigned int i_p;             // -- plate index value
    double relative_vel_cm[3];    /* M/s
         velocity of vehicle center of mass relative to ambient atmosphere in
         inertial Reference Frame */
    double rel_vel_cm_struct[3];  /* M/s
       velocity of vehicle center of mass relative to ambient atmosphere in
       Structural Reference Frame */
    double rel_vel_struct_hat[3]; /* M/s
      unit vector associated with rel_vel_cm_struct */
    double rel_vel_mag;           /* M/s
               magnitude of rel_vel_cm_struct */

    // If aerodynamics is not needed, get out
    if(!active)
    {
        return;
    }

    // Safety: initialize values to zero
    Vector3::initialize(aero_force);
    Vector3::initialize(aero_torque);

    Vector3::diff(inertial_velocity, atmos_ptr->wind, relative_vel_cm);
    // Transform relative velocity to structural reference frame and calculate the
    // magnitude and unit vector of the velocity in the structural frame
    Vector3::transform(T_inertial_struct, relative_vel_cm, rel_vel_cm_struct);

    rel_vel_mag = Vector3::vmag(rel_vel_cm_struct);
    if(std::fpclassify(rel_vel_mag) != FP_ZERO)
    {
        double inv_vel = 1 / rel_vel_mag; /* s/M inverse of rel_vel_mag */
        Vector3::scale(rel_vel_cm_struct, inv_vel, rel_vel_struct_hat);
    }
    else
    {
        Vector3::unit(1, rel_vel_struct_hat);
    }
    // dynamic pressure is the kinetic energy density of the free-moelcular flow

    if(constant_density == false)
    {
        density = atmos_ptr->density;
    }
    param.dynamic_pressure = 0.5 * density * rel_vel_mag * rel_vel_mag;

    // decide if you will be using the full plate model, or the simple default
    // behavior
    if(use_default_behavior == true)
    {
        default_behavior->aerodrag_force(rel_vel_mag, rel_vel_struct_hat, &param, mass, aero_force, aero_torque);
        return;
    }

    // If we are to this point, we NEED to have a pointer to the aero_surface
    if(aero_surface_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             AerodynamicsMessages::runtime_error,
                             "use_default_behavior was set to false, but no aero_surface_ptr "
                             "was supplied. Please supply an aero_surface_ptr.");
        return;
    }

    for(i_p = 0; i_p < aero_surface_ptr->facets_size; ++i_p)
    {
        Vector3::initialize(aero_surface_ptr->aero_facets[i_p]->force);
        Vector3::initialize(aero_surface_ptr->aero_facets[i_p]->torque);
    }
    // Compute the aerodynamic forces on each plate.
    for(i_p = 0; i_p < aero_surface_ptr->facets_size; ++i_p)
    {
        aero_surface_ptr->aero_facets[i_p]->aerodrag_force(rel_vel_mag, rel_vel_struct_hat, &param, center_grav);
        // sum individual plate forces over all plates.
        Vector3::incr(aero_surface_ptr->aero_facets[i_p]->force, aero_force);
        Vector3::incr(aero_surface_ptr->aero_facets[i_p]->torque, aero_torque);
    }
}

/**
 * Set the surface this AeroDrag object will calculate drag for.
 * \param[in] to_set The AeroSurface to be used
 */
void AerodynamicDrag::set_aero_surface(AeroSurface & to_set)
{
    aero_surface_ptr = &to_set;
}

/**
 * Remove any AeroSurface being used for calculation. Note: The variable
 * "use_default_behavior" must be set to true if there is no set
 * aero surface
 */
void AerodynamicDrag::clear_aero_surface()
{
    aero_surface_ptr = nullptr;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
