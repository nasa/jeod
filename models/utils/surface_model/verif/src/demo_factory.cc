/************************** TRICK HEADER***************************************
PURPOSE:
    (Used in the surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
   ((demo_factory.cc)
    (demo_interaction_facet.cc)
    (utils/surface_model/src/flat_plate.cc)
    (utils/sim_interface/src/memory_interface.cc))

 
*******************************************************************************/


// System includes
#include <cstddef>
#include <typeinfo>

// JEOD includes
#include "utils/surface_model/include/flat_plate.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/demo_factory.hh"
#include "../include/demo_params.hh"
#include "../include/demo_interaction_facet.hh"
#include "../include/demo_facet.hh"


//! Namespace jeod
namespace jeod {

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES (FlatPlateDemo1)
JEOD_DECLARE_ATTRIBUTES (FlatPlateDemo2)
JEOD_DECLARE_ATTRIBUTES (DemoInteractionFacet1)
JEOD_DECLARE_ATTRIBUTES (DemoInteractionFacet2)



FlatPlateDemoFactory1::FlatPlateDemoFactory1(){
   JEOD_REGISTER_CLASS(FlatPlateDemo1);
}

FlatPlateDemoFactory1::~FlatPlateDemoFactory1(){

   // empty for now

}

InteractionFacet* FlatPlateDemoFactory1::create_facet(
   Facet* facet,
   FacetParams* params){

   FlatPlateDemoParams1* demo_params = nullptr;
   FlatPlate* flat_plate = nullptr;

   demo_params = dynamic_cast<FlatPlateDemoParams1*>(params);
   flat_plate = dynamic_cast<FlatPlate*>(facet);

   if(demo_params == nullptr){
      // INSERT ERROR MESSAGE HERE
   }
   if(flat_plate == nullptr){
      // INSERT ERROR MESSAGE HERE
   }

   FlatPlateDemo1* inter_facet = JEOD_ALLOC_CLASS_OBJECT(FlatPlateDemo1, ());

   // This is a quick and dirty shallow copy. It is NOT normally
   // suggested to copy a string like this.
   inter_facet->shape = demo_params->shape;

   inter_facet->base_facet = facet;

   return inter_facet;

}

bool FlatPlateDemoFactory1::is_correct_factory(Facet* facet){

   if(typeid(*facet) == typeid(FlatPlate)) {
      return true;
   }
   else {
      return false;
   }

}

FlatPlateDemoFactory2::FlatPlateDemoFactory2(){

   JEOD_REGISTER_CLASS(FlatPlateDemo2);

}

FlatPlateDemoFactory2::~FlatPlateDemoFactory2(){

   // empty for now

}

InteractionFacet* FlatPlateDemoFactory2::create_facet(
   Facet* facet,
   FacetParams* params){

   FlatPlateDemoParams2* demo_params = nullptr;
   FlatPlate* flat_plate = nullptr;

   demo_params = dynamic_cast<FlatPlateDemoParams2*>(params);
   flat_plate = dynamic_cast<FlatPlate*>(facet);

   if(demo_params == nullptr){
      // INSERT ERROR MESSAGE HERE
   }
   if(flat_plate == nullptr){
      // INSERT ERROR MESSAGE HERE
   }

   FlatPlateDemo2* inter_facet = JEOD_ALLOC_CLASS_OBJECT(FlatPlateDemo2, ());

   inter_facet->sides = demo_params->sides;

   inter_facet->base_facet = facet;

   return inter_facet;

}

bool FlatPlateDemoFactory2::is_correct_factory(Facet* facet){

   if(typeid(*facet) == typeid(FlatPlate)) {
      return true;
   }
   else {
      return false;
   }

}

DemoFacetFactory1::DemoFacetFactory1(){

   JEOD_REGISTER_CLASS(DemoInteractionFacet1);

}

DemoFacetFactory1::~DemoFacetFactory1(){

   // empty for now

}

InteractionFacet* DemoFacetFactory1::create_facet(
   Facet* facet,
   FacetParams* params){

   DemoParams1* demo_params = nullptr;
   DemoFacet* flat_plate = nullptr;

   demo_params = dynamic_cast<DemoParams1*>(params);
   flat_plate = dynamic_cast<DemoFacet*>(facet);

   if(demo_params == nullptr){
      // INSERT ERROR MESSAGE HERE
   }
   if(flat_plate == nullptr){
      // INSERT ERROR MESSAGE HERE
   }

   DemoInteractionFacet1* inter_facet = JEOD_ALLOC_CLASS_OBJECT(DemoInteractionFacet1, ());

   inter_facet->weight = demo_params->weight;

   inter_facet->base_facet = facet;

   return inter_facet;

}

bool DemoFacetFactory1::is_correct_factory(Facet* facet){

   if(typeid(*facet) == typeid(DemoFacet)) {
      return true;
   }
   else {
      return false;
   }

}

DemoFacetFactory2::DemoFacetFactory2(){

   JEOD_REGISTER_CLASS(DemoInteractionFacet2);

}

DemoFacetFactory2::~DemoFacetFactory2(){

   // empty for now

}

InteractionFacet* DemoFacetFactory2::create_facet(
   Facet* facet,
   FacetParams* params){

   DemoParams2* demo_params = nullptr;
   DemoFacet* flat_plate = nullptr;

   demo_params = dynamic_cast<DemoParams2*>(params);
   flat_plate = dynamic_cast<DemoFacet*>(facet);

   if(demo_params == nullptr){
      // INSERT ERROR MESSAGE HERE
   }
   if(flat_plate == nullptr){
      // INSERT ERROR MESSAGE HERE
   }

   DemoInteractionFacet2* inter_facet = JEOD_ALLOC_CLASS_OBJECT(DemoInteractionFacet2, ());

   // This is a quick and dirty shallow copy. This is not how you should
   // normally copy a string.
   inter_facet->color = demo_params->color;

   inter_facet->base_facet = facet;

   return inter_facet;

}

bool DemoFacetFactory2::is_correct_factory(Facet* facet){

   if(typeid(*facet) == typeid(DemoFacet)) {
      return true;
   }
   else {
      return false;
   }

}
} // End JEOD namespace
