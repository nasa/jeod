/************************** TRICK HEADER***************************************
PURPOSE:
    (Demonstration surface for surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    ((demo_surface.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// Model includes
#include "../include/demo_interaction.hh"

//! Namespace jeod
namespace jeod
{

void DemoInteraction1::run_interaction(DemoSurface1 & surface)
{
    for(unsigned int ii = 0; ii < surface.facets_size; ++ii)
    {
        surface.demo_facets[ii]->execute_demo_1(interaction_number);
    }
}

void DemoInteraction2::run_interaction(DemoSurface2 & surface)
{
    for(unsigned int ii = 0; ii < surface.facets_size; ++ii)
    {
        surface.demo_facets[ii]->execute_demo_2(interaction_name.c_str());
    }
}

} // namespace jeod
