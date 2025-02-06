/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_pressure__surface_model.cc
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
     (radiation_surface.o)
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
#include "../include/radiation_surface.hh"
#include "../include/radiation_third_body.hh"
#include "../include/radiation_source.hh"



//! Namespace jeod
namespace jeod {

/******************************************************************************/
/******************************************************************************/


/**
 * Initialize the radiation pressure model when using a RadiationSurface (i.e.
 * with facets).
 * \param[in] dyn_mgr The dynamics manager
 * \param[in] surf_ptr pointer to the radiation surface
 * \param[in] center_grav position of center of mass\n Units: M
 */
/******************************************************************************/


/******************************************************************************
  Purpose:
    (Initialize the radiation pressure model when using a RadiationSurface (i.e.
    with facets).)
******************************************************************************/
void
RadiationPressure::initialize (
   DynManager & dyn_mgr,
   RadiationSurface * surf_ptr,
   double center_grav[3])
{
   // make the model thermally dynamic as default
   thermal.active = true;

   // set the Dyn Manager pointer
   dyn_manager_ptr = &(dyn_mgr);

   // initialize the 3rd bodies (provides compatibility with Trick7 methodology
   // of defining 3rd bodies in default data)
   for (unsigned int ii_body = 0; ii_body < source.num_bodies; ++ii_body) {
      add_third_body(source.third_body[ii_body]);
      int old_shadow_geom = source.shadow_geometry;
      third_bodies.back()->convert_shadow_from_int(old_shadow_geom);
   }

   // initialize the primary source.
   initialize_environment (dyn_manager_ptr);

   // initialize the surface.
   surface_ptr = surf_ptr;
   surface_ptr->initialize (center_grav);


   return;
}



/**
 * Used to update the model when the surface comprises facets.
 */
void
RadiationPressure::update_facet_surface (
   void)
{
   // initialize each of the facets here, because paths diverge after this.
   surface_ptr->initialize_runtime_values();

   if (source.flux_mag > 0) {
      surface_ptr->incident_radiation (source.flux_mag,
                                       source.flux_struc_hat,
                                       calculate_forces);
   }

   for (unsigned int ii_body = 0; ii_body < num_third_bodies; ++ii_body) {
      if (third_bodies[ii_body]->is_interactive() &&
          third_bodies[ii_body]->active) {
         surface_ptr->interact_with_third_body( third_bodies[ii_body],
                                                calculate_forces);
      }
   }

   //  Note: must always do the thermal update, even if it thermal.active =
   //  false.  In that case (i.e.  facet temperatures are stationary)
   //  there will still be radiative thermal emission corresponding
   //  to the radiation absorption.  This generates a force which must be
   //  included in the radiation pressure calculation.
   thermal.update (surface_ptr);


   // If the thermal model is active, individual inactive facets have already had
   // power_absorb set to power_emit in the thermal.integrate function, called
   // from the previous thermal.update() call.  If not,
   // that must still be carried out for all facets.
   if (!thermal.active) {
      surface_ptr->equalize_absorption_emission();
   }

   // Forces are not always desired, this model can be used to obtain irradiance
   // measurements for lighting models, electrical subsystems (solar panels),
   // thermal subsystems.  But typically, this flag is true.
   if (calculate_forces) {
      surface_ptr->radiation_pressure();
      Vector3::copy (surface_ptr->force,
                     force);
      Vector3::copy (surface_ptr->torque,
                     torque);
   }


   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
