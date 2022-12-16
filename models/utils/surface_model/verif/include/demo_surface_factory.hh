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
    (demo_surface_factory.o)

 
*******************************************************************************/

#ifndef JEOD_DEMO_SURFACE_FACTORY_HH
#define JEOD_DEMO_SURFACE_FACTORY_HH

#include "utils/surface_model/include/interaction_surface_factory.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/surface_model.hh"
#include "utils/surface_model/include/interaction_surface.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"

// Need to do something with this?
// #include "flat_plate_aero_factory.hh"

#include <vector>

//! Namespace jeod
namespace jeod {

class DemoSurfaceFactory1 : public InteractionSurfaceFactory {

   JEOD_MAKE_SIM_INTERFACES(DemoSurfaceFactory1)

public:

   // constructor
   DemoSurfaceFactory1();

   // destructor
   virtual ~DemoSurfaceFactory1();

   virtual void add_facet_params(FacetParams* to_add);


protected:

  // Included so JEOD can include all default facet factories that
  // it knows about. For extensibility the user can add
  // factories with add_facet_factory
  // probably can be taken out....
  // FlatPlateAeroFactory flat_plate_aero_factory;

private:

   // operator = and copy constructor locked from use because they
   // are declared private

   DemoSurfaceFactory1& operator = (const DemoSurfaceFactory1& rhs);
   DemoSurfaceFactory1(const DemoSurfaceFactory1& rhs);

};

class DemoSurfaceFactory2 : public InteractionSurfaceFactory {

   JEOD_MAKE_SIM_INTERFACES(DemoSurfaceFactory2)

public:

   // constructor
   DemoSurfaceFactory2();

   // destructor
   virtual ~DemoSurfaceFactory2();

   virtual void add_facet_params(FacetParams* to_add);


protected:

  // Included so JEOD can include all default facet factories that
  // it knows about. For extensibility the user can add
  // factories with add_facet_factory
  // probably can be taken out....
  // FlatPlateAeroFactory flat_plate_aero_factory;

private:

   // operator = and copy constructor locked from use because they
   // are declared private

   DemoSurfaceFactory2& operator = (const DemoSurfaceFactory2& rhs);
   DemoSurfaceFactory2(const DemoSurfaceFactory2& rhs);

};

} // End JEOD namespace

#endif
