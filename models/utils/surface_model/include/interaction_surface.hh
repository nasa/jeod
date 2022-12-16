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
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/include/interaction_surface.hh
 * Vehicle surface model for general environment interaction models.
 * This is a pure virtual function and has methods that must be
 * implemented in any inheritied, instantiable class
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((interaction_surface.o))

 
*******************************************************************************/

#ifndef JEOD_INTERACTION_SURFACE_HH
#define JEOD_INTERACTION_SURFACE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes


//! Namespace jeod
namespace jeod {

class Facet;
class InteractionFacetFactory;
class FacetParams;

/**
 * A base class for interaction specific surfaces.
 */
class InteractionSurface {

   JEOD_MAKE_SIM_INTERFACES(InteractionSurface)

public:

   // constructor
   InteractionSurface ();

   // destructor
   virtual ~InteractionSurface ();

   // Used when thermal sources are spread among many different interactions
      /**
    * Adds all thermal sources together.
    */
   /*
     Purpose:
       (Adds all thermal sources together.)
    */
   virtual void
   accumulate_thermal_sources (
      void)
   {};
   // Used to integrate thermal sources over time to get temperature
      /**
    * Integrates thermal sources to get temperature.
    */
   /*
     Purpose:
       (Integrates thermal sources to get temperature.)
    */
   virtual void
   thermal_integrator (
      void)
   {};

   /**
    * A pure virtual function that will allocate the array of
    * pointers to the correct interaction facet type, of the given
    * size
    * \param[in] size Size of the array to be allocated\n Units: cnt
    */

   virtual void allocate_array(
      unsigned int size)
 = 0; /* In: cnt Size of the array to be allocated */

   /**
    * A pure virtual function that will allocate the interaction
    * facet, from the given facet, using the given facet parameters,
    * and place it in the allocated array of interaction facets
    * at the given index
    * \param[in] facet The facet used to create the interaction facet
    * \param[in] factory The factory used to create the interaction facet
    * \param[in] params The parameters used to create the interaction facet
    * \param[in] index Where in the interaction facet array the interaction facet will be placed\n Units: cnt
    */
   virtual void allocate_interaction_facet(
      Facet* facet,
      InteractionFacetFactory* factory,
      FacetParams* params,
      unsigned int index)
 = 0; /* In: cnt Where ine the interaction facet
                                      array the interaction facet will be
                                      placed */



protected:


private:

   // operator = and copy constructor locked from use because they
   // are declared private

   InteractionSurface& operator = (const InteractionSurface& rhs);
   InteractionSurface (const InteractionSurface& rhs);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
