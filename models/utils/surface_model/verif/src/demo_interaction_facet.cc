/************************** TRICK HEADER***************************************
PURPOSE:
    (Used for the surface model verification)

Library dependencies:
    ((demo_interaction_facet.cc))


*******************************************************************************/

// System includes
#include <cstddef>
#include <cstdio>

// JEOD includes
#include "utils/surface_model/include/flat_plate.hh"

// Model includes
#include "../include/demo_facet.hh"
#include "../include/demo_interaction_facet.hh"

//! Namespace jeod
namespace jeod
{

void FlatPlateDemo1::execute_demo_1(unsigned int interaction_number)
{
    auto * base_plate = dynamic_cast<FlatPlate *>(base_facet);

    fprintf(stderr, "\n\n\n");
    fprintf(stderr, "FlatPlateDemo1::execute_demo_1\n");
    fprintf(stderr, "The interaction number is: %d\n", interaction_number);
    fprintf(stderr, "The shape is: %s\n", shape.c_str());
    fprintf(stderr, "Area of flat plate is: %f\n", base_plate->area);
    fprintf(stderr,
            "normal of flat plate is: %f %f %f\n",
            base_plate->normal[0],
            base_plate->normal[1],
            base_plate->normal[2]);
}

void FlatPlateDemo2::execute_demo_2(const char * interaction_name)
{
    auto * base_plate = dynamic_cast<FlatPlate *>(base_facet);
    fprintf(stderr, "\n\n\n");
    fprintf(stderr, "FlatPlateDemo2::execute_demo_2\n");
    fprintf(stderr, "The interaction name is: %s\n", interaction_name);
    fprintf(stderr, "The number of sides is: %d\n", sides);
    fprintf(stderr, "Area of flat plate is: %f\n", base_plate->area);
    fprintf(stderr,
            "normal of flat plate is: %f %f %f\n",
            base_plate->normal[0],
            base_plate->normal[1],
            base_plate->normal[2]);
}

void DemoInteractionFacet1::execute_demo_1(unsigned int interaction_number)
{
    auto * base_plate = dynamic_cast<DemoFacet *>(base_facet);
    fprintf(stderr, "\n\n\n");
    fprintf(stderr, "DemoInteractionFacet1::execute_demo_1\n");
    fprintf(stderr, "The interaction number is: %d\n", interaction_number);
    fprintf(stderr, "The weight is: %f\n", weight);
    fprintf(stderr, "the name of the DemoFacet is: %s\n", base_plate->name.c_str());
    fprintf(stderr, "the semi-random int of the DemoFacet is: %d\n", base_plate->some_int);
}

void DemoInteractionFacet2::execute_demo_2(const char * interaction_name)
{
    auto * base_plate = dynamic_cast<DemoFacet *>(base_facet);
    fprintf(stderr, "\n\n\n");
    fprintf(stderr, "DemoInteractionFacet2::execute_demo_2\n");
    fprintf(stderr, "The interaction name is: %s\n", interaction_name);
    fprintf(stderr, "The color is: %s\n", color.c_str());
    fprintf(stderr, "the name of the DemoFacet is: %s\n", base_plate->name.c_str());
    fprintf(stderr, "the semi-random int of the DemoFacet is: %d\n", base_plate->some_int);
}
} // namespace jeod
