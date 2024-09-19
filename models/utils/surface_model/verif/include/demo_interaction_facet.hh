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
    (Used for the surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    (../src/demo_interaction_facet.cc)


*******************************************************************************/

#ifndef JEOD_DEMO_INTERACTION_FACET_HH
#define JEOD_DEMO_INTERACTION_FACET_HH

#include <string>

#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/interaction_facet.hh"

//! Namespace jeod
namespace jeod
{

class DemoFacet1 : public InteractionFacet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoFacet1)

public:
    DemoFacet1() = default;
    ~DemoFacet1() override = default;
    DemoFacet1 & operator=(const DemoFacet1 &) = delete;
    DemoFacet1(const DemoFacet1 &) = delete;

    virtual void execute_demo_1(unsigned int interaction_number) = 0;
};

class DemoFacet2 : public InteractionFacet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoFacet2)

public:
    DemoFacet2() = default;
    ~DemoFacet2() override = default;
    DemoFacet2 & operator=(const DemoFacet2 &) = delete;
    DemoFacet2(const DemoFacet2 &) = delete;

    virtual void execute_demo_2(const char * interaction_name) = 0;
};

class FlatPlateDemo1 : public DemoFacet1
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateDemo1)

public:
    FlatPlateDemo1() = default;
    ~FlatPlateDemo1() override = default;
    FlatPlateDemo1 & operator=(const FlatPlateDemo1 &) = delete;
    FlatPlateDemo1(const FlatPlateDemo1 &) = delete;

    void execute_demo_1(unsigned int interaction_number) override;

    std::string shape; // (--) The shape of the flat plate
};

class FlatPlateDemo2 : public DemoFacet2
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateDemo2)

public:
    FlatPlateDemo2() = default;
    ~FlatPlateDemo2() override = default;
    FlatPlateDemo2 & operator=(const FlatPlateDemo2 &) = delete;
    FlatPlateDemo2(const FlatPlateDemo2 &) = delete;

    void execute_demo_2(const char * interaction_name) override;

    int sides{}; // (count) Number of sides
};

class DemoInteractionFacet1 : public DemoFacet1
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoInteractionFacet1)

public:
    DemoInteractionFacet1() = default;
    ~DemoInteractionFacet1() override = default;
    DemoInteractionFacet1 & operator=(const DemoInteractionFacet1 &) = delete;
    DemoInteractionFacet1(const DemoInteractionFacet1 &) = delete;

    void execute_demo_1(unsigned int interaction_number) override;

    double weight{}; // N The weight of the facet
};

class DemoInteractionFacet2 : public DemoFacet2
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoInteractionFacet2)

public:
    DemoInteractionFacet2() = default;
    ~DemoInteractionFacet2() override = default;
    DemoInteractionFacet2 & operator=(const DemoInteractionFacet2 &) = delete;
    DemoInteractionFacet2(const DemoInteractionFacet2 &) = delete;

    void execute_demo_2(const char * interaction_name) override;

    std::string color{}; // -- The shape of the facet
};

} // namespace jeod

#endif
