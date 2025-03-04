//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
namespace jeod
{

class FlatPlateDemoFactory1 : public InteractionFacetFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateDemoFactory1)

public:
    FlatPlateDemoFactory1();
    ~FlatPlateDemoFactory1() override = default;
    FlatPlateDemoFactory1 & operator=(const FlatPlateDemoFactory1 &) = delete;
    FlatPlateDemoFactory1(const FlatPlateDemoFactory1 &) = delete;

    InteractionFacet * create_facet(Facet * facet, FacetParams * params) override;

    bool is_correct_factory(Facet * facet) override;
};

class FlatPlateDemoFactory2 : public InteractionFacetFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateDemoFactory2)

public:
    FlatPlateDemoFactory2();
    ~FlatPlateDemoFactory2() override = default;
    FlatPlateDemoFactory2 & operator=(const FlatPlateDemoFactory2 &) = delete;
    FlatPlateDemoFactory2(const FlatPlateDemoFactory2 &) = delete;

    InteractionFacet * create_facet(Facet * facet, FacetParams * params) override;

    bool is_correct_factory(Facet * facet) override;
};

class DemoFacetFactory1 : public InteractionFacetFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoFacetFactory1)

public:
    DemoFacetFactory1();
    ~DemoFacetFactory1() override = default;
    DemoFacetFactory1 & operator=(const FlatPlateDemoFactory2 &) = delete;
    DemoFacetFactory1(const FlatPlateDemoFactory2 &) = delete;

    InteractionFacet * create_facet(Facet * facet, FacetParams * params) override;

    bool is_correct_factory(Facet * facet) override;
};

class DemoFacetFactory2 : public InteractionFacetFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoFacetFactory2)

public:
    DemoFacetFactory2();
    ~DemoFacetFactory2() override = default;
    DemoFacetFactory2 & operator=(const FlatPlateDemoFactory2 &) = delete;
    DemoFacetFactory2(const FlatPlateDemoFactory2 &) = delete;

    InteractionFacet * create_facet(Facet * facet, FacetParams * params) override;

    bool is_correct_factory(Facet * facet) override;
};

} // namespace jeod

#include "demo_interaction_facet.hh"

#endif
