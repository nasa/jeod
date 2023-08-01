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

#include "../include/demo_surface_factory.hh"
#include "../include/demo_params.hh"


//! Namespace jeod
namespace jeod {

DemoSurfaceFactory1::DemoSurfaceFactory1()
{
  // empty for now
}

DemoSurfaceFactory1::~DemoSurfaceFactory1(){

   // empty for now

}

void DemoSurfaceFactory1::add_facet_params(FacetParams* to_add){

   DemoParams* temp_ptr = nullptr;

   temp_ptr = dynamic_cast<DemoParams*> (to_add);

   if(temp_ptr == nullptr){

      // INSERT ERROR MESSAGE HERE

      return;
   }

   InteractionSurfaceFactory::add_facet_params(to_add);

   return;

}

DemoSurfaceFactory2::DemoSurfaceFactory2()
{
  // empty for now
}

DemoSurfaceFactory2::~DemoSurfaceFactory2(){

   // empty for now

}

void DemoSurfaceFactory2::add_facet_params(FacetParams* to_add){

   DemoParams* temp_ptr = nullptr;

   temp_ptr = dynamic_cast<DemoParams*> (to_add);

   if(temp_ptr == nullptr){

      // INSERT ERROR MESSAGE HERE

      return;
   }

   InteractionSurfaceFactory::add_facet_params(to_add);

   return;

}
} // End JEOD namespace
