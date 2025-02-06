/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_pressure__default_surface.cc
 * Calculation of force and torque due to radiation pressure
 */

/******************************* TRICK HEADER *********************************
PURPOSE:
    ()
REFERENCE:
    (((None)))
ASSUMPTIONS AND LIMITATIONS:
    ((Orbital body modeled as a collection of facets)
     (Includes an integration of temperature; assumes that the next time step is
     equal to the previous time step.  If this is not true, it will be corrected
     at the next calculation.))
CLASS:
    (scheduled)
LIBRARY DEPENDENCY:
    ((radiation_pressure.o)
     (radiation_source.o)
     (radiation_default_surface.o)
     (radiation_third_body.o)
     (interactions/thermal_rider/thermal_model_rider.o))



******************************************************************************/

// System includes

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/math/include/vector3.hh"
#include "interactions/thermal_rider/include/thermal_model_rider.hh"

// Model structure includes
#include "../include/radiation_pressure.hh"
#include "../include/radiation_default_surface.hh"
#include "../include/radiation_third_body.hh"
#include "../include/radiation_source.hh"



//! Namespace jeod
namespace jeod {

/******************************************************************************/
/******************************************************************************/

/**
 * Initialize the radiation pressure model when using a
 * RadiationDefaultSurface (i.e. spherical, uniform properties.)
 * \param[in] dyn_mgr The dynamics manager
 * \param[in] surf_ptr pointer to the radiation surface
 */
/******************************************************************************/

/******************************************************************************
  Purpose:
    (Initialize the radiation pressure model when using a
    RadiationDefaultSurface (i.e. spherical, uniform properties.))
******************************************************************************/
void
RadiationPressure::initialize (
   DynManager & dyn_mgr,
   RadiationDefaultSurface * surf_ptr)
{

   dyn_manager_ptr = &(dyn_mgr);

   // initialize the 3rd bodies (provides compatibility with Trick7 methodology
   // of defining 3rd bodies in default data)
   for (unsigned int ii_body = 0; ii_body < source.num_bodies; ++ii_body) {
      add_third_body(source.third_body[ii_body]);
      int old_shadow_geom = source.shadow_geometry;
      third_bodies.back()->convert_shadow_from_int(old_shadow_geom);
   }

   initialize_environment (dyn_manager_ptr);
   default_surface_ptr = surf_ptr;
   default_surface_ptr->initialize();
   return;
}

/**
 * Used to update the model when the surface is a default surface.
 */
void
RadiationPressure::update_default_surface (
   void)
{
   default_surface_ptr->initialize_runtime_values();

   if (source.flux_mag > 0) {
      default_surface_ptr->incident_radiation (source.flux_mag,
                                               source.flux_struc_hat,
                                               calculate_forces);
   }

   for (unsigned int ii_body = 0; ii_body < num_third_bodies; ++ii_body) {
      if (third_bodies[ii_body]->is_interactive() &&
          third_bodies[ii_body]->active) {
         default_surface_ptr->interact_with_third_body( third_bodies[ii_body],
                                                        calculate_forces);
      }
   }

   default_surface_ptr->thermal_update();

   if (calculate_forces) {
      Vector3::sum ( default_surface_ptr->F_absorption,
                     default_surface_ptr->F_specular,
                     default_surface_ptr->F_diffuse,
                     force);
   }
   Vector3::initialize (torque);


   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
