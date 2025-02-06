//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
    (Used in the surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    (../src/demo_factory.cc)

 
*******************************************************************************/

#ifndef DEMO_FACTORY_HH
#define DEMO_FACTORY_HH

#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/interaction_facet.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"

//! Namespace jeod
namespace jeod {

class FlatPlateDemoFactory1 : public InteractionFacetFactory {

   JEOD_MAKE_SIM_INTERFACES(FlatPlateDemoFactory1)

public:

   // constructor
   FlatPlateDemoFactory1();

   // destructor
   ~FlatPlateDemoFactory1() override;

   InteractionFacet* create_facet(Facet* facet,
                                          FacetParams* params) override;

   bool is_correct_factory(Facet* facet) override;

protected:

private:

   // operator = and copy constructor locked from use because they
   // are declared private

   FlatPlateDemoFactory1& operator = (const FlatPlateDemoFactory1& rhs);
   FlatPlateDemoFactory1(const FlatPlateDemoFactory1& rhs);


};

class FlatPlateDemoFactory2 : public InteractionFacetFactory {

   JEOD_MAKE_SIM_INTERFACES(FlatPlateDemoFactory2)

public:

   // constructor
   FlatPlateDemoFactory2();

   // destructor
   ~FlatPlateDemoFactory2() override;

   InteractionFacet* create_facet(Facet* facet,
                                          FacetParams* params) override;

   bool is_correct_factory(Facet* facet) override;

protected:

private:

   // operator = and copy constructor locked from use because they
   // are declared private

   FlatPlateDemoFactory2& operator = (const FlatPlateDemoFactory2& rhs);
   FlatPlateDemoFactory2(const FlatPlateDemoFactory2& rhs);


};

 class DemoFacetFactory1 : public InteractionFacetFactory {

   JEOD_MAKE_SIM_INTERFACES(DemoFacetFactory1)

public:

   // constructor
   DemoFacetFactory1();

   // destructor
   ~DemoFacetFactory1() override;

   InteractionFacet* create_facet(Facet* facet,
                                          FacetParams* params) override;

   bool is_correct_factory(Facet* facet) override;

protected:

private:

   // operator = and copy constructor locked from use because they
   // are declared private

   DemoFacetFactory1& operator = (const FlatPlateDemoFactory2& rhs);
   DemoFacetFactory1(const FlatPlateDemoFactory2& rhs);


};

class DemoFacetFactory2 : public InteractionFacetFactory {

   JEOD_MAKE_SIM_INTERFACES(DemoFacetFactory2)

public:

   // constructor
   DemoFacetFactory2();

   // destructor
   ~DemoFacetFactory2() override;

   InteractionFacet* create_facet(Facet* facet,
                                          FacetParams* params) override;

   bool is_correct_factory(Facet* facet) override;

protected:

private:

   // operator = and copy constructor locked from use because they
   // are declared private

   DemoFacetFactory2& operator = (const FlatPlateDemoFactory2& rhs);
   DemoFacetFactory2(const FlatPlateDemoFactory2& rhs);


};

} // End JEOD namespace

#include "demo_interaction_facet.hh"

#endif
