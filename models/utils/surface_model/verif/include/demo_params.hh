//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
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

#ifndef DEMO_PARAMS_HH
#define DEMO_PARAMS_HH

#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/facet_params.hh"

//! Namespace jeod
namespace jeod {

class DemoParams : public FacetParams {

   JEOD_MAKE_SIM_INTERFACES(DemoParams)

public:

   // constructor
   DemoParams();

   // destructor
   virtual ~DemoParams();


protected:

   DemoParams& operator = (const DemoParams& rhs);
   DemoParams(const DemoParams& rhs);

private:


};

class DemoParams1 : public DemoParams {

   JEOD_MAKE_SIM_INTERFACES(DemoParams1)

public:

   // constructor
   DemoParams1();

   // destructor
   virtual ~DemoParams1();

   double weight; // (N) a weight, just to have something to do

protected:

private:

   DemoParams1& operator = (const DemoParams1& rhs);
   DemoParams1(const DemoParams1& rhs);

};

class DemoParams2 : public DemoParams {

   JEOD_MAKE_SIM_INTERFACES(DemoParams2)

public:

   // constructor
   DemoParams2();

   // destructor
   virtual ~DemoParams2();

   char* color; // (--) The color of the thing

protected:

private:

   DemoParams2& operator = (const DemoParams2& rhs);
   DemoParams2(const DemoParams2& rhs);

};

class FlatPlateDemoParams1 : public DemoParams {

   JEOD_MAKE_SIM_INTERFACES(FlatPlateDemoParams1)

public:

   // constructor
   FlatPlateDemoParams1();

   // destructor
   virtual ~FlatPlateDemoParams1();

   char* shape; // (--) The shape of the thing

protected:

private:

   FlatPlateDemoParams1& operator = (const FlatPlateDemoParams1& rhs);
   FlatPlateDemoParams1(const FlatPlateDemoParams1& rhs);

};

class FlatPlateDemoParams2 : public DemoParams {

   JEOD_MAKE_SIM_INTERFACES(FlatPlateDemoParams2)

public:

   // constructor
   FlatPlateDemoParams2();

   // destructor
   virtual ~FlatPlateDemoParams2();

   int sides; // (count) The number of sides of the plate

protected:

private:

   FlatPlateDemoParams2& operator = (const FlatPlateDemoParams2& rhs);
   FlatPlateDemoParams2(const FlatPlateDemoParams2& rhs);

};

} // End JEOD namespace

#endif
