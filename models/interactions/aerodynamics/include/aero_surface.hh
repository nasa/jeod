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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/include/aero_surface.hh
 * Vehicle surface model for aerodynamics
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((aero_surface.o))


*******************************************************************************/

#ifndef JEOD_AERO_SURFACE_HH
#define JEOD_AERO_SURFACE_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/interaction_surface.hh"

// Model includes

//! Namespace jeod
namespace jeod {

class Facet;
class AeroFacet;
class InteractionFacetFactory;


/**
 * The aerodynamic specific interaction surface, for use with the surface model.
 */
class AeroSurface : public InteractionSurface {

   JEOD_MAKE_SIM_INTERFACES(AeroSurface)

public:

   // constructor
   AeroSurface ();

   // destructor
   virtual ~AeroSurface ();

   /**
    * An array of pointers to aerodynamic interaction facets.
    * AeroFacets is a pure virtual, so these will all
    * be pointed to inheriting classes through polymorphism
    */
   AeroFacet** aero_facets; //!< trick_units(--)

   /**
    * Size of the aero_facets array
    */
   unsigned int facets_size; //!< trick_units(count)

   // Allocates the aero_facets array from the given size
   virtual void allocate_array (unsigned int size);

   // Allocates the facet at the "index" value in aero_facets, using
   // the base Facet given by the pointer facet, and using the parameter
   // object pointed to by params pointer and using the
   // InteractionFacetFactory pointed to by factory.
   virtual void allocate_interaction_facet (
      Facet* facet,
      InteractionFacetFactory* factory,
      FacetParams* params,
      unsigned int index);


protected:


private:

   // operator = and copy constructor locked from use because they
   // are declared private

   AeroSurface& operator = (const AeroSurface& rhs);
   AeroSurface (const AeroSurface& rhs);

};

} // End JEOD namespace

#ifdef TRICK_VER
#include "aero_facet.hh"
#include "utils/surface_model/include/facet.hh"
#endif
#endif

/**
 * @}
 * @}
 * @}
 */
