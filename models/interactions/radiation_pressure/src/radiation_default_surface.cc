/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_default_surface.cc
 * Default surface for use with Radiation Pressure interaction model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((radiation_default_surface.cc)
(radiation_messages.cc)
(radiation_third_body.cc)
(interactions/thermal_rider/src/thermal_facet_rider.cc)
(utils/message/src/message_handler.cc))



*******************************************************************************/

/* System includes */

/*  JEOD includes */
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "interactions/thermal_rider/include/thermal_facet_rider.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

/* Model structure includes */
#include "../include/radiation_default_surface.hh"
#include "../include/radiation_messages.hh"
#include "../include/radiation_third_body.hh"


//! Namespace jeod
namespace jeod {

//Static default values.

//const double RadiationDefaultSurface::speed_of_light = 299792458.0;

/**
 * Constructor for RadiationDefaultSurface
 */

//const double RadiationDefaultSurface::speed_of_light = 299792458.0;

/******************************************************************************
Purpose:
(Constructor for RadiationDefaultSurface)
******************************************************************************/
RadiationDefaultSurface::RadiationDefaultSurface (
   void)
{
   cx_area      = 0.0;
   surface_area = 0.0;
   areaxflux    = 0.0;
   areaxflux_e  = 0.0;
   albedo       = -1.0;
   diffuse      = -1.0;
   temperature  = 0.0;
   rad_coeff    = -1.0;

   Vector3::initialize (F_absorption);
   Vector3::initialize (F_diffuse);
   Vector3::initialize (F_specular);
   Vector3::initialize (F_emission);

}


/**
 * initializes the default surface
 */
void
RadiationDefaultSurface::initialize (
   void)
{
   double epsilon = 1.0E-12;

   RadiationBaseFacet::initialize();

   if ((albedo < 0.0) && (diffuse < 0.0) && (rad_coeff < 0.0)) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "The surface reflectance has not been defined correctly.\n"
         "Either the reflection (a.k.a. radiation) coefficient (Cr), or\n"
         "both the albedo and diffuse parameters must be specified \n"
         "and be within their appropriate ranges \n"
         "(rad_coeff between 1.0 and 1.444,\n"
         "albedo and diffuse both between 0.0 and 1.0).\n");
   }


   if (albedo * diffuse < 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "If specifying the surface reflectance by an albedo & diffuse \n"
         "combination, both must be specified within the range 0 to 1.\n");
   }

   if ((rad_coeff > 0) && (albedo > 0)) { // hence diffuse must be > 0 also
      // tried to define material properties and their combined value.
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "Both the radiation coefficient (rad_coeff) and either or both of\n"
         "the albedo and diffuse parameters were defined.\n"
         "Specify either radiation coefficient, or albedo and diffuse.\n");
   }

   if (rad_coeff > 0) { // defined by rad_coeff, albedo and diffuse not defined
      if ((rad_coeff > (13.0 / 9.0)) || (rad_coeff < 1.0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
            "The radiation coefficient (rad_coeff) was specified with a value\n"
            "outside its legitimate range.\n"
            "The radiation coefficient represents a multiplicative factor that\n"
            "allows a reflective surface to be represented as a larger\n"
            "non-reflective surface (and experience the same force).\n"
            "For a spherical surface (such as this default surface), the \n"
            "legitimate range of that value is from 1.0 to 1.44444  (i.e. 13/9).\n");
      }
      albedo  = 9 * (rad_coeff - 1) / 4;
      diffuse = 1.0;
      if (albedo > 1.0) { // This cannot happen, but just in case
         MessageHandler::warn (
            __FILE__, __LINE__, RadiationMessages::operational_setup_error,
            "\n"
            "Radiation Coefficient was defined, and albedo was calculated \n"
            "from it to have value %f, which is outside its range.  \n"
            "Adjusting value of albedo to 1.0.\n", albedo);
         albedo = 1.0;
      }
      if (albedo < 0.0) { // This cannot happen, but just in case
         MessageHandler::warn (
            __FILE__, __LINE__, RadiationMessages::operational_setup_error,
            "\n"
            "Radiation Coefficient was defined, and albedo was calculated \n"
            "from it to have value %f, which is outside its range.  \n"
            "Adjusting value of albedo to 0.0.\n", albedo);
         albedo = 0.0;
      }
   }
   else { // defined by albedo and diffuse
      if ((albedo > 1.0) || (albedo < 0.0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
            "The albedo of the default surface is out of range.  \n"
            "Albedo represents a fraction of incident light, and must \n"
            "be between 0 and 1.\n");
      }
      if ((diffuse > 1.0) || (diffuse < 0.0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
            "The diffuse parameter for the default surface is out of range.  \n"
            "Diffuse represents a fraction of reflected light, and must \n"
            "be between 0 and 1.\n");
      }
   }

   // Area definitions


   if ((std::fpclassify(cx_area) != FP_ZERO) && (std::fpclassify(surface_area) != FP_ZERO)) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "Both the cross-sectional area (cx_area) and surface-area (surface_area)"
         "\n were specified.  Only one should be specified; its value is used to"
         "\n calculate the other.\n");
   }

   if (cx_area < epsilon) {
      cx_area = surface_area / 4.0;
   }
   if (surface_area < epsilon) {
      surface_area = cx_area * 4.0;
   }
   if (cx_area < epsilon) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "The area of the default surface is 0.0.  This is usually due to an \n"
         "omission in defining the surface parameters.\n"
         "One of the cross-sectional area (cx_area) or surface-area (surface_area)"
         "\n should be specified to be non-zero.\n");
   }

   thermal.initialize (temperature, surface_area);
   return;
}



/**
 * Calculation of force and torque due to radiation pressure
 *
 * \par Assumptions and Limitations
 *  - Assumes that flux magnitude is positive.
 *  - Will only be called if flux magnitude is positive.
 *  - Assumes spherical surface.
 * \param[in] flux_mag the magnitude of the incident flux
 * \param[in] flux_struc_hat the unit vector in structural frame for the flux vector.
 * \param[in] calculate_forces boolean indicating whether to calculate forces.
 */
void
RadiationDefaultSurface::incident_radiation (
   const double flux_mag,
   const double flux_struc_hat[3],
   const bool calculate_forces)
{
   // energy flux
   areaxflux_e           = cx_area * flux_mag;
   thermal.power_absorb += (1.0 - albedo) * areaxflux_e;

   if (!calculate_forces) {
      return;
   }

   // momentum flux
   areaxflux = areaxflux_e / speed_of_light;

   double tempvec1[3];
   // absorption
   Vector3::scale (flux_struc_hat,
                   areaxflux * (1.0 - albedo),
                   tempvec1);
   Vector3::incr( tempvec1,
                  F_absorption);

   // reflection
   double ref_flux = areaxflux * albedo;

   // Diffuse reflection
   Vector3::scale (flux_struc_hat,
                   ref_flux * diffuse * 13.0 / 9.0,
                   tempvec1);
   Vector3::incr( tempvec1,
                  F_diffuse);

   // Specular reflection
   Vector3::scale (flux_struc_hat,
                   ref_flux * (1.0 - diffuse),
                   tempvec1);
   Vector3::incr( tempvec1,
                  F_specular);


   return;
}



/**
 * Test for necessity of maintaining thermal updates, and performs them as
 * necessary
 */
void
RadiationDefaultSurface::thermal_update(
   void)

{
   if (thermal.active) {
      thermal.power_absorb += thermal.thermal_power_dump;
      if (std::fpclassify(ThermalFacetRider::cycle_time) != FP_ZERO) {
         temperature = thermal.integrate();
      }
   }
   return;
}

/**
 * Adds the thermal integrator of this surface to the integration group
 * of the DynBody
 * \param[in,out] dyn_body Body associated with this thermal object.
 */
void
RadiationDefaultSurface::add_thermal_integrator_to (
   DynBody * dyn_body)
{
   dyn_body->add_integrable_object (thermal.integrable_object);
}


/**
 * Destructor for RadiationDefaultSurface
 */
RadiationDefaultSurface::~RadiationDefaultSurface (
   void)
{
   // empty for now
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
