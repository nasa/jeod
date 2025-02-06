/************************** TRICK HEADER***************************************
PURPOSE:
    (Used for the surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    (demo_facet.o)

 
*******************************************************************************/

// System includes
#include <cstddef>

// Model includes
#include "../include/demo_facet.hh"


//! Namespace jeod
namespace jeod {

DemoFacet::DemoFacet() :
   name(),
   some_int(0)
{

   // empty for now
}

DemoFacet::~DemoFacet(){

   // empty for now

}
} // End JEOD namespace
