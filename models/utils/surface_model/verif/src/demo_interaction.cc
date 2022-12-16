/************************** TRICK HEADER***************************************
PURPOSE:
    (Demonstration surface for surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    ((demo_surface.o)
     (utils/sim_interface/memory_interface.o))

 
*******************************************************************************/

// System includes
#include <cstddef>

// Model includes
#include "../include/demo_interaction.hh"


//! Namespace jeod
namespace jeod {

DemoInteraction1::DemoInteraction1() :
   interaction_number(0)
{

   // empty for now

}

DemoInteraction1::~DemoInteraction1(){


}

void DemoInteraction1::run_interaction(
   DemoSurface1& surface)
{
   for(unsigned int ii = 0; ii < surface.facets_size; ++ii){
      surface.demo_facets[ii]->execute_demo_1(interaction_number);
   }
   return;

}



DemoInteraction2::DemoInteraction2() :
   interaction_name(NULL)
{

   // empty for now

}

DemoInteraction2::~DemoInteraction2(){

}

void DemoInteraction2::run_interaction(
   DemoSurface2& surface)
{
   for(unsigned int ii = 0; ii < surface.facets_size; ++ii){
      surface.demo_facets[ii]->execute_demo_2(interaction_name);
   }

   return;

}
} // End JEOD namespace
