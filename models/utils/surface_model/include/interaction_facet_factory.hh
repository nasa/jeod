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
 * @file models/utils/surface_model/include/interaction_facet_factory.hh
 * Factory that creates an interaction facet, for a specific
 * environment interaction model, from a facet model. This is a
 * pure virtual class, and the pure virtual functions must be
 * implemented in any instantiable, inheriting class
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((interaction_facet_factory.o))

 
*******************************************************************************/

#ifndef JEOD_INTERACTION_FACET_FACTORY_HH
#define JEOD_INTERACTION_FACET_FACTORY_HH


// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

class Facet;
class FacetParams;
class InteractionFacet;

// Model includes



/**
 * A factory to create a specific interaction facet from a general facet.
 */
class InteractionFacetFactory {

   JEOD_MAKE_SIM_INTERFACES(InteractionFacetFactory)

public:

   // constructor
   InteractionFacetFactory ();

   // destructor
   virtual ~InteractionFacetFactory ();

   /**
    * A pure virtual function that creates a specific interaction
    * facet from a base facet with the given FacetParams. This
    * defines interface for all classes that inherit from
    * InteractionFacetFactory
    * @return The new interaction facet
    * \param[in] facet The facet the InteractionFacet is created from
    * \param[in] params The parameter object to be added.
    */

   virtual InteractionFacet* create_facet(
      Facet* facet,
      FacetParams* params)
 = 0; /* In: -- The specific parameters used to
                                   create the interaction facet */

   /**
    * A pure virtual function. Returns true or false: is the given facet
    * the type this factory is meant to use?
    * @return true or false. Is the given facet the correct type for this factory?
    * \param[in] facet The facet that is being checked
    */

   virtual bool is_correct_factory(
      Facet* facet)
 = 0; /* In: -- The facet that is being checked */

protected:

   /**
    * Unused data field to expedite dynamic allocation in Trick environment
    */
   bool trick_bool; //!< trick_units(--)

private:

   // operator = and copy constructor locked from use because they
   // are declared private

   InteractionFacetFactory& operator = (const InteractionFacetFactory& rhs);
   InteractionFacetFactory (const InteractionFacetFactory& rhs);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
