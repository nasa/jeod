/************************** TRICK HEADER***************************************
PURPOSE:
    (RadiationData recorder through facet pointers)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    ((radiation_data_recorder.cc)
     (interactions/radiation_pressure/src/flat_plate_radiation_facet.cc))


*******************************************************************************/

// JEOD includes
#include "interactions/radiation_pressure/include/flat_plate_radiation_facet.hh"
#include "interactions/radiation_pressure/include/radiation_surface.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/facet.hh"

// Model includes
#include "../include/radiation_data_recorder.hh"

//! Namespace jeod
namespace jeod
{

/******************************************************************************/
/******************************************************************************/
void RadiationDataRecorder::record_data( // Return: -- void
    RadiationSurface * surface_ptr)
{
    for(unsigned int ii_facet = 0; ii_facet < surface_ptr->num_facets; ++ii_facet)
    {
        for(unsigned int ii = 0; ii < 3; ++ii)
        {
            F_absorption[ii_facet][ii] = surface_ptr->facets[ii_facet]->F_absorption[ii];
            F_diffuse[ii_facet][ii] = surface_ptr->facets[ii_facet]->F_diffuse[ii];
            F_emission[ii_facet][ii] = surface_ptr->facets[ii_facet]->F_emission[ii];
            F_specular[ii_facet][ii] = surface_ptr->facets[ii_facet]->F_specular[ii];
            force[ii_facet][ii] = surface_ptr->facets[ii_facet]->force[ii];
            torque[ii_facet][ii] = surface_ptr->facets[ii_facet]->torque[ii];
        }
        temperature[ii_facet] = surface_ptr->facets[ii_facet]->base_facet->temperature;
        power_absorb[ii_facet] = surface_ptr->facets[ii_facet]->thermal.power_absorb;
        power_emit[ii_facet] = surface_ptr->facets[ii_facet]->thermal.power_emit;
    }
}

} // namespace jeod
