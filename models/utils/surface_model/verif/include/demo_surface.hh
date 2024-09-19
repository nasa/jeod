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
    (Interaction Surfaces for demosntration of surface model)

REFERENCE:
    (None)

ASSUMPTIONS AND LIMITATIONS:
      (None)

Library dependencies:
    (../src/demo_surface.cc)


*******************************************************************************/

#ifndef JEOD_DEMO_SURFACES_HH
#define JEOD_DEMO_SURFACES_HH

#include "demo_interaction_facet.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"
#include "utils/surface_model/include/interaction_surface.hh"

//! Namespace jeod
namespace jeod
{

class DemoSurface1 : public InteractionSurface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoSurface1)

public:
    DemoSurface1();
    ~DemoSurface1() override;
    DemoSurface1 & operator=(const DemoSurface1 &) = delete;
    DemoSurface1(const DemoSurface1 &) = delete;

    DemoFacet1 ** demo_facets{}; /* trick_units(--) @n
        Array of facets to be tested */

    unsigned int facets_size{}; /* trick_units(count) @n
       Size of the demo_facets array */

    void allocate_array(unsigned int size) override;
    void allocate_interaction_facet(Facet * facet,
                                    InteractionFacetFactory * factory,
                                    FacetParams * params,
                                    unsigned int index) override;
};

class DemoSurface2 : public InteractionSurface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DemoSurface2)

public:
    DemoSurface2();
    ~DemoSurface2() override;
    DemoSurface2 & operator=(const DemoSurface2 &) = delete;
    DemoSurface2(const DemoSurface2 &) = delete;

    DemoFacet2 ** demo_facets{}; /* trick_units(--) @n
        Array of facets to be tested */

    unsigned int facets_size{}; /* trick_units(count) @n
       Size of the demo_facets array */

    void allocate_array(unsigned int size) override;
    void allocate_interaction_facet(Facet * facet,
                                    InteractionFacetFactory * factory,
                                    FacetParams * params,
                                    unsigned int index) override;
};

} // namespace jeod

#endif
