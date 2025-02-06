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

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/demo_surface.hh"

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(DemoFacet1)
JEOD_DECLARE_ATTRIBUTES(DemoFacet2)

DemoSurface1::DemoSurface1()
{
    JEOD_REGISTER_INCOMPLETE_CLASS(DemoFacet1);
}

DemoSurface1::~DemoSurface1()
{
    JEOD_DELETE_2D(demo_facets, facets_size, false);
}

void DemoSurface1::allocate_array(unsigned int size)
{
    if(demo_facets != nullptr)
    {
        // INSERT ERROR MESSAGE HERE
        return;
    }

    demo_facets = JEOD_ALLOC_CLASS_POINTER_ARRAY(size, DemoFacet1);
    facets_size = size;

    // Make sure all pointers are NULL so destructor never crashes
    for(unsigned int ii = 0; ii < facets_size; ++ii)
    {
        demo_facets[ii] = nullptr;
    }
}

void DemoSurface1::allocate_interaction_facet(Facet * facet,
                                              InteractionFacetFactory * factory,
                                              FacetParams * params,
                                              unsigned int index)
{
    if(facets_size <= index)
    {
        // INSERT ERROR MESSAGE HERE

        return;
    }

    /* need to temporarily save off the InteractionFacet returned before
       dynamic casting it. If the dynamic cast fails, we want to destroy
       the InteractionFacet so we don't get a memory leak */

    InteractionFacet * temp_facet = nullptr;
    temp_facet = factory->create_facet(facet, params);

    if(temp_facet == nullptr)
    {
        // INSERT ERROR MESSAGE HERE

        return;
    }

    auto * temp_demo_facet = dynamic_cast<DemoFacet1 *>(temp_facet);

    if(temp_demo_facet == nullptr)
    {
        // temp_facet can NOT be NULL, since it was already checked for above
        JEOD_DELETE_OBJECT(temp_facet);

        // INSERT ERROR MESSAGE HERE

        return;
    }

    demo_facets[index] = temp_demo_facet;
}

DemoSurface2::DemoSurface2()
{
    JEOD_REGISTER_INCOMPLETE_CLASS(DemoFacet2);
}

DemoSurface2::~DemoSurface2()
{
    JEOD_DELETE_2D(demo_facets, facets_size, false);
}

void DemoSurface2::allocate_array(unsigned int size)
{
    if(demo_facets != nullptr)
    {
        // INSERT ERROR MESSAGE HERE
        return;
    }

    demo_facets = JEOD_ALLOC_CLASS_POINTER_ARRAY(size, DemoFacet2);
    facets_size = size;

    // Make sure all pointers are NULL so destructor never crashes
    for(unsigned int ii = 0; ii < facets_size; ++ii)
    {
        demo_facets[ii] = nullptr;
    }
}

void DemoSurface2::allocate_interaction_facet(Facet * facet,
                                              InteractionFacetFactory * factory,
                                              FacetParams * params,
                                              unsigned int index)
{
    if(facets_size <= index)
    {
        // INSERT ERROR MESSAGE HERE

        return;
    }

    /* need to temporarily save off the InteractionFacet returned before
       dynamic casting it. If the dynamic cast fails, we want to destroy
       the InteractionFacet so we don't get a memory leak */

    InteractionFacet * temp_facet = nullptr;
    temp_facet = factory->create_facet(facet, params);

    if(temp_facet == nullptr)
    {
        // INSERT ERROR MESSAGE HERE

        return;
    }

    auto * temp_demo_facet = dynamic_cast<DemoFacet2 *>(temp_facet);

    if(temp_demo_facet == nullptr)
    {
        // temp_facet can NOT be NULL, since it was already checked for above
        JEOD_DELETE_OBJECT(temp_facet);

        // INSERT ERROR MESSAGE HERE

        return;
    }

    demo_facets[index] = temp_demo_facet;
}
} // namespace jeod
