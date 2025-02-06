/************************** TRICK HEADER***************************************
PURPOSE:
    (Used for the surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    (demo_params.o)

 
*******************************************************************************/

#include <cstddef>

#include "../include/demo_params.hh"


//! Namespace jeod
namespace jeod {

DemoParams::DemoParams(){

   // empty for now

}

DemoParams::~DemoParams(){

   // empty for now

}

DemoParams1::DemoParams1() :
   weight(0.0)
{
   // empty for now
}

DemoParams1::~DemoParams1(){

   // empty for now

}

DemoParams2::DemoParams2() :
   color(NULL)
{
   // empty for now
}

DemoParams2::~DemoParams2(){

   // empty for now

}

FlatPlateDemoParams1::FlatPlateDemoParams1() :
   shape(NULL)
{
   // empty for now
}

FlatPlateDemoParams1::~FlatPlateDemoParams1(){

   // empty for now

}

FlatPlateDemoParams2::FlatPlateDemoParams2() :
   sides(0)
{
   // empty for now
}

FlatPlateDemoParams2::~FlatPlateDemoParams2(){

   // empty for now

}
} // End JEOD namespace
