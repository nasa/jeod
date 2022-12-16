/************************** TRICK HEADER***************************************
PURPOSE:
    (Used for the surface model verification)

Library dependencies:
    ((demo_interaction_facet.o))

 
*******************************************************************************/


// System includes
#include <cstddef>
#include <cstdio>

// JEOD includes
#include "utils/surface_model/include/flat_plate.hh"

// Model includes
#include "../include/demo_interaction_facet.hh"
#include "../include/demo_facet.hh"


//! Namespace jeod
namespace jeod {

DemoFacet1::DemoFacet1 (
   void)
{

}


DemoFacet1::~DemoFacet1 ( // Return: -- void
   void)
{
   // empty for now

}

DemoFacet2::DemoFacet2 (
   void)
{

}


DemoFacet2::~DemoFacet2 ( // Return: -- void
   void)
{
   // empty for now

}

FlatPlateDemo1::FlatPlateDemo1 (
   void) :
   shape(NULL)
{

}


FlatPlateDemo1::~FlatPlateDemo1 ( // Return: -- void
   void)
{
   // empty for now

}

void FlatPlateDemo1::execute_demo_1 (
   unsigned int interaction_number){
   FlatPlate* base_plate = dynamic_cast<FlatPlate*> (base_facet);

   fprintf(stderr, "\n\n\n");
   fprintf(stderr, "FlatPlateDemo1::execute_demo_1\n");
   fprintf(stderr, "The interaction number is: %d\n", interaction_number);
   fprintf(stderr, "The shape is: %s\n", shape);
   fprintf(stderr, "Area of flat plate is: %f\n", base_plate->area);
   fprintf(stderr, "normal of flat plate is: %f %f %f\n",
      base_plate->normal[0],
      base_plate->normal[1],
      base_plate->normal[2]);

   return;
}

FlatPlateDemo2::FlatPlateDemo2 () :
   sides(0)
{

}


FlatPlateDemo2::~FlatPlateDemo2 ( // Return: -- void
   void)
{
   // empty for now

}

void FlatPlateDemo2::execute_demo_2 (
   char* interaction_name){
   FlatPlate* base_plate = dynamic_cast<FlatPlate*> (base_facet);
   fprintf(stderr, "\n\n\n");
   fprintf(stderr, "FlatPlateDemo2::execute_demo_2\n");
   fprintf(stderr, "The interaction name is: %s\n", interaction_name);
   fprintf(stderr, "The number of sides is: %d\n", sides);
   fprintf(stderr, "Area of flat plate is: %f\n", base_plate->area);
   fprintf(stderr, "normal of flat plate is: %f %f %f\n",
      base_plate->normal[0],
      base_plate->normal[1],
      base_plate->normal[2]);

}

DemoInteractionFacet1::DemoInteractionFacet1 (
   void) :
   weight(0)
{

}


DemoInteractionFacet1::~DemoInteractionFacet1 ( // Return: -- void
   void)
{
   // empty for now

}

void DemoInteractionFacet1::execute_demo_1 (
   unsigned int interaction_number){
   DemoFacet* base_plate = dynamic_cast<DemoFacet*> (base_facet);
   fprintf(stderr, "\n\n\n");
   fprintf(stderr, "DemoInteractionFacet1::execute_demo_1\n");
   fprintf(stderr, "The interaction number is: %d\n", interaction_number);
   fprintf(stderr, "The weight is: %f\n", weight);
   fprintf(stderr, "the name of the DemoFacet is: %s\n", base_plate->name.c_str());
   fprintf(stderr, "the semi-random int of the DemoFacet is: %d\n",
      base_plate->some_int);

}

DemoInteractionFacet2::DemoInteractionFacet2 (
   void) :
   color(NULL)
{

}


DemoInteractionFacet2::~DemoInteractionFacet2 ( // Return: -- void
   void)
{
   // empty for now

}

void DemoInteractionFacet2::execute_demo_2 (
   char* interaction_name){
   DemoFacet* base_plate = dynamic_cast<DemoFacet*> (base_facet);
   fprintf(stderr, "\n\n\n");
   fprintf(stderr, "DemoInteractionFacet2::execute_demo_2\n");
   fprintf(stderr, "The interaction name is: %s\n", interaction_name);
   fprintf(stderr, "The color is: %s\n", color);
   fprintf(stderr, "the name of the DemoFacet is: %s\n", base_plate->name.c_str());
   fprintf(stderr, "the semi-random int of the DemoFacet is: %d\n",
      base_plate->some_int);

}
} // End JEOD namespace
