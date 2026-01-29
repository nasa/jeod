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

static FILE * demoOutputFp = stderr;

void set_demo_output_file(const std::string & fname)
{
    if(!fname.empty())
    {
        demoOutputFp = fopen(fname.c_str(), "w");
    }
}

void FlatPlateDemo1::execute_demo_1(unsigned int interaction_number)
{
    auto * base_plate = dynamic_cast<FlatPlate *>(base_facet);

    fprintf(demoOutputFp, "\n\n\n");
    fprintf(demoOutputFp, "FlatPlateDemo1::execute_demo_1\n");
    fprintf(demoOutputFp, "The interaction number is: %d\n", interaction_number);
    fprintf(demoOutputFp, "The shape is: %s\n", shape.c_str());
    fprintf(demoOutputFp, "Area of flat plate is: %f\n", base_plate->area);
    fprintf(demoOutputFp,
            "normal of flat plate is: %f %f %f\n",
            base_plate->normal[0],
            base_plate->normal[1],
            base_plate->normal[2]);
}

void FlatPlateDemo2::execute_demo_2(const char * interaction_name)
{
    auto * base_plate = dynamic_cast<FlatPlate *>(base_facet);
    fprintf(demoOutputFp, "\n\n\n");
    fprintf(demoOutputFp, "FlatPlateDemo2::execute_demo_2\n");
    fprintf(demoOutputFp, "The interaction name is: %s\n", interaction_name);
    fprintf(demoOutputFp, "The number of sides is: %d\n", sides);
    fprintf(demoOutputFp, "Area of flat plate is: %f\n", base_plate->area);
    fprintf(demoOutputFp,
            "normal of flat plate is: %f %f %f\n",
            base_plate->normal[0],
            base_plate->normal[1],
            base_plate->normal[2]);
}

void DemoInteractionFacet1::execute_demo_1(unsigned int interaction_number)
{
    auto * base_plate = dynamic_cast<DemoFacet *>(base_facet);
    fprintf(demoOutputFp, "\n\n\n");
    fprintf(demoOutputFp, "DemoInteractionFacet1::execute_demo_1\n");
    fprintf(demoOutputFp, "The interaction number is: %d\n", interaction_number);
    fprintf(demoOutputFp, "The weight is: %f\n", weight);
    fprintf(demoOutputFp, "the name of the DemoFacet is: %s\n", base_plate->name.c_str());
    fprintf(demoOutputFp, "the semi-random int of the DemoFacet is: %d\n", base_plate->some_int);
}

void DemoInteractionFacet2::execute_demo_2(const char * interaction_name)
{
    auto * base_plate = dynamic_cast<DemoFacet *>(base_facet);
    fprintf(demoOutputFp, "\n\n\n");
    fprintf(demoOutputFp, "DemoInteractionFacet2::execute_demo_2\n");
    fprintf(demoOutputFp, "The interaction name is: %s\n", interaction_name);
    fprintf(demoOutputFp, "The color is: %s\n", color.c_str());
    fprintf(demoOutputFp, "the name of the DemoFacet is: %s\n", base_plate->name.c_str());
    fprintf(demoOutputFp, "the semi-random int of the DemoFacet is: %d\n", base_plate->some_int);
}

} // namespace jeod
