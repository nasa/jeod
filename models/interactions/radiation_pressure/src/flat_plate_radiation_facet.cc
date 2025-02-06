/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/flat_plate_radiation_facet.cc
 * Define member functions for class FlatPlateRadiationFacet
 */

/*****************************************************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((Facet is a flat plate with no geometry))

LIBRARY DEPENDENCY:
((flat_plate_radiation_facet.o)
(radiation_facet.o)
(radiation_messages.o)
(interactions/thermal_rider/thermal_facet_rider.o)
(utils/message/message_handler.o))



******************************************************************************/


// System includes

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/surface_model/include/flat_plate.hh"

// Model includes
#include "../include/flat_plate_radiation_facet.hh"
#include "../include/radiation_messages.hh"
#include "../include/radiation_third_body.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a FlatPlateRadiationFacet
 */
FlatPlateRadiationFacet::FlatPlateRadiationFacet
(
   void) : // Return: -- void
   normal(NULL)
{
   sin_theta = 0.0;
   Vector3::initialize(incident_flux_hat);

}


/**
 * Defines the facet data values
 * \param[in] flat_plate pointer to the flat plate object
 */
void
FlatPlateRadiationFacet::define_facet (
   FlatPlate * flat_plate)
{
   // already done RadiationFacet values, which includes position.
   // point the local center of pressure and normal to the correct
   // flat plate values.  Note that these values are dynamic if
   // the surface is articulated, so their values cannot be copied over.
   center_pressure = flat_plate->position;
   normal = flat_plate->normal;

   return;
}





/**
 * Initializes the Facet for use in the model
 * \param[in] center_grav center of gravity position\n Units: M
 */
void
FlatPlateRadiationFacet::initialize_geom (
   double center_grav[3])
{
   RadiationFacet::initialize();

   thermal.initialize (base_facet->temperature, base_facet->area);
   Vector3::diff (center_pressure,
                  center_grav,
                  crot_to_cp);
   return;
}

/**
 * Calculation of force and torque due to radiation pressure
 *
 * \par Assumptions and Limitations
 *  - Only called when flux_mag > 0
 * \param[in] flux_mag incident flux (per unit area)\n Units: N/m
 * \param[in] flux_struct_hat the flux unit vector in structural frame
 * \param[in] calculate_forces on/off flag for whether to calculate forces.
 */
void
FlatPlateRadiationFacet::incident_radiation (
   const double flux_mag,
   const double flux_struct_hat[3],
   const bool calculate_forces)
{
   sin_theta = -(Vector3::dot (normal, flux_struct_hat));
   if (sin_theta < 0) { /* plate is not illuminated */
      return;
   }
   cx_area               = base_facet->area * sin_theta;

   areaxflux_e           = cx_area * flux_mag;
   thermal.power_absorb += (1.0 - albedo) * areaxflux_e;

   if (!calculate_forces) {
      return;
   }

   // Convert to momentum
   areaxflux = areaxflux_e / speed_of_light;

   double tempvec1[3];
   double tempvec2[3];
   // absorption
   Vector3::scale (flux_struct_hat,
                   areaxflux * (1.0 - albedo),
                   tempvec1);
   Vector3::incr( tempvec1,
                  F_absorption);

   // reflection
   double ref_flux =  areaxflux * albedo; // total reflected flux

   // diffuse reflection
   Vector3::scale ( normal,
                    two_thirds,
                    tempvec1),
                    Vector3::diff (  flux_struct_hat,
                                     tempvec1,
                                     tempvec2 ); // tempvec2 = phi_hat - 2/3 n_hat
   Vector3::scale ( diffuse * ref_flux,
                    tempvec2);
   Vector3::incr (  tempvec2,
                    F_diffuse);

   // specular reflection */
   Vector3::scale (normal,
                   2 * (diffuse - 1) * ref_flux * sin_theta,
                   tempvec1); // (diffuse-1) < 0, this makes F opposite to
   // normal
   Vector3::incr ( tempvec1,
                   F_specular);

   return;
}



/**
 * Calculates the radiative emission force, accumulated force, and torque
 * acting on a facet.
 */
void
FlatPlateRadiationFacet::radiation_pressure (
   void)
{
   if (thermal.integrable_object.active) {
      thermal.integrable_object.compute_temp_dot();
   }

   if (thermal.power_emit < 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::unknown_numerical_error, "\n"
         "On facet(%s), the calculation of emitted power yielded negative emission"
         ",\n which is a non-physical situation corresponding to emission \n"
         "producing a net gain in thermal energy.\n", base_facet->name.c_str());
   }
   else {
      // If there is some thermal emission, convert it to an emission force and
      // add it to the total plate force to give the final plate force
      Vector3::scale (normal,
                      -two_thirds * thermal.power_emit / speed_of_light,
                      F_emission);
   }
   //  Combining, accumulating
   Vector3::sum ( F_absorption,
                  F_specular,
                  F_diffuse,
                  force);
   Vector3::incr (F_emission,
                  force);

   // - Reminder crot_to_cp is the moment arm from cg (center of rotation) to cp
   // (center of pressure - effective center of facet)
   Vector3::cross (crot_to_cp,
                   force,
                   torque);

   return;
}

/**
 * Destructor for FlatPlateRadiationFacet
 */
FlatPlateRadiationFacet::~FlatPlateRadiationFacet (
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
