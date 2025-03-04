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
    (Used for the surface model verification)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

*******************************************************************************/

#ifndef DEMO_PARAMS_HH
#define DEMO_PARAMS_HH

#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/facet_params.hh"

//! Namespace jeod
namespace jeod
{

class DemoParams : public FacetParams
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoParams)

public:
    DemoParams() = default;
    ~DemoParams() override = default;
    DemoParams & operator=(const DemoParams &) = delete;
    DemoParams(const DemoParams &) = delete;
};

class DemoParams1 : public DemoParams
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoParams1)

public:
    DemoParams1() = default;
    ~DemoParams1() override = default;
    DemoParams1 & operator=(const DemoParams1 &) = delete;
    DemoParams1(const DemoParams1 &) = delete;

    double weight{}; // (N) a weight, just to have something to do
};

class DemoParams2 : public DemoParams
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoParams2)

public:
    DemoParams2() = default;
    ~DemoParams2() override = default;
    DemoParams2 & operator=(const DemoParams2 &) = delete;
    DemoParams2(const DemoParams2 &) = delete;

    std::string color; // (--) The color of the thing
};

class FlatPlateDemoParams1 : public DemoParams
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateDemoParams1)

public:
    FlatPlateDemoParams1() = default;
    ~FlatPlateDemoParams1() override = default;
    FlatPlateDemoParams1 & operator=(const FlatPlateDemoParams1 &) = delete;
    FlatPlateDemoParams1(const FlatPlateDemoParams1 &) = delete;

    std::string shape; // (--) The shape of the thing
};

class FlatPlateDemoParams2 : public DemoParams
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateDemoParams2)

public:
    FlatPlateDemoParams2() = default;
    ~FlatPlateDemoParams2() override = default;
    FlatPlateDemoParams2 & operator=(const FlatPlateDemoParams2 &) = delete;
    FlatPlateDemoParams2(const FlatPlateDemoParams2 &) = delete;

    int sides{}; // (count) The number of sides of the plate
};

} // namespace jeod

#endif
