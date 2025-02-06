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
    (demo_interaction_facet.o)

 
*******************************************************************************/

#ifndef JEOD_DEMO_INTERACTION_FACET_HH
#define JEOD_DEMO_INTERACTION_FACET_HH

#include "utils/sim_interface/include/jeod_class.hh"

#include "utils/surface_model/include/interaction_facet.hh"

//! Namespace jeod
namespace jeod {

class DemoFacet1 : public InteractionFacet {

   JEOD_MAKE_SIM_INTERFACES(DemoFacet1)

public:

   // constructor
   DemoFacet1();

   // destructor
   virtual ~DemoFacet1();

   virtual void execute_demo_1(
      unsigned int interaction_number
      ) = 0;


protected:

private:

   DemoFacet1& operator = (const DemoFacet1& rhs);
   DemoFacet1(const DemoFacet1& rhs);

};

class DemoFacet2 : public InteractionFacet {

   JEOD_MAKE_SIM_INTERFACES(DemoFacet2)

public:

   // constructor
   DemoFacet2();

   // destructor
   virtual ~DemoFacet2();

   virtual void execute_demo_2(
      char* interaction_name
      ) = 0;


protected:

private:

   DemoFacet2& operator = (const DemoFacet2& rhs);
   DemoFacet2(const DemoFacet2& rhs);

};

class FlatPlateDemo1 : public DemoFacet1 {

   JEOD_MAKE_SIM_INTERFACES(FlatPlateDemo1)

public:

   // constructor
   FlatPlateDemo1();

   // destructor
   virtual ~FlatPlateDemo1();

   virtual void execute_demo_1(
      unsigned int interaction_number) ;

   char* shape; // (--) The shape of the flat plate

protected:

private:

   FlatPlateDemo1& operator = (const FlatPlateDemo1& rhs);
   FlatPlateDemo1(const FlatPlateDemo1& rhs);

};

class FlatPlateDemo2 : public DemoFacet2 {

   JEOD_MAKE_SIM_INTERFACES(FlatPlateDemo2)

public:

   // constructor
   FlatPlateDemo2();

   // destructor
   virtual ~FlatPlateDemo2();

   virtual void execute_demo_2(
      char* interaction_name);

   int sides; // (count) Number of sides

protected:

private:

   FlatPlateDemo2& operator = (const FlatPlateDemo2& rhs);
   FlatPlateDemo2(const FlatPlateDemo2& rhs);

};

class DemoInteractionFacet1 : public DemoFacet1 {

   JEOD_MAKE_SIM_INTERFACES(DemoInteractionFacet1)

public:

   // constructor
   DemoInteractionFacet1();

   // destructor
   virtual ~DemoInteractionFacet1();

   virtual void execute_demo_1(
      unsigned int interaction_number);

   double weight; // N The weight of the facet

protected:

private:

   DemoInteractionFacet1& operator = (const DemoInteractionFacet1& rhs);
   DemoInteractionFacet1(const DemoInteractionFacet1& rhs);

};

class DemoInteractionFacet2 : public DemoFacet2 {

   JEOD_MAKE_SIM_INTERFACES(DemoInteractionFacet2)

public:

   // constructor
   DemoInteractionFacet2();

   // destructor
   virtual ~DemoInteractionFacet2();

   virtual void execute_demo_2(
      char* interaction_name);

   char* color; // -- The shape of the facet

protected:

private:

   DemoInteractionFacet2& operator = (const DemoInteractionFacet2& rhs);
   DemoInteractionFacet2(const DemoInteractionFacet2& rhs);

};

} // End JEOD namespace

#endif
