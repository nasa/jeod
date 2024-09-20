/************************** TRICK HEADER***************************************
PURPOSE:
    (Used for the surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    (demo_surface_factory.cc)


*******************************************************************************/

#include <cstddef>

#include "../include/demo_params.hh"
#include "../include/demo_surface_factory.hh"

//! Namespace jeod
namespace jeod
{

void DemoSurfaceFactory1::add_facet_params(FacetParams * to_add)
{
    DemoParams * temp_ptr = nullptr;

    temp_ptr = dynamic_cast<DemoParams *>(to_add);

    if(temp_ptr == nullptr)
    {
        // INSERT ERROR MESSAGE HERE

        return;
    }

    InteractionSurfaceFactory::add_facet_params(to_add);
}

void DemoSurfaceFactory2::add_facet_params(FacetParams * to_add)
{
    DemoParams * temp_ptr = nullptr;

    temp_ptr = dynamic_cast<DemoParams *>(to_add);

    if(temp_ptr == nullptr)
    {
        // INSERT ERROR MESSAGE HERE

        return;
    }

    InteractionSurfaceFactory::add_facet_params(to_add);
}
} // namespace jeod
