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
/*****************************************************************************

 Purpose:
    (ground facets for use with contact interaction models)

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

 Library dependencies:
    ((ground_facet.o))

 

*****************************************************************************/

#ifndef GROUND_FACET_HH
#define GROUND_FACET_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "interactions/contact/include/class_declarations.hh"

#include "ground_interaction.hh"

//! Namespace jeod
namespace jeod {

/*
  Purpose:
    (An contact interaction specific facet for use in the surface model.)
 */
class GroundFacet {

   JEOD_MAKE_SIM_INTERFACES(GroundFacet)

public:
   bool active; /* trick_units(--) @n
      toggles this contact facet on and off, true=on false=off */

   double alt_offset; /* trick_units(m) @n
      altitude offset used to positioning objects for dynamics effect and
      graphics sync. */

   ContactParams * surface_type; /* trick_units(--) @n
      Stores the name of surface material that the facet is constructed of.  This
      information is used to deturmine the contact parameters used when pairs
      are constructed. */

   // constructor
   GroundFacet ();

   // destructor
   virtual ~GroundFacet ();

   /*
    Overloaded function to create a GroundInteraction of the appropriate type and return
    a pointer to the Contact class.
    */
   virtual GroundInteraction * create_interaction(ContactFacet * subject, Contact * contact);

private:
   /* Operator = and copy constructor hidden from use by being private */

   GroundFacet& operator = (const GroundFacet& rhs);
   GroundFacet (const GroundFacet& rhs);

};

} // End JEOD namespace

#if (defined TRICK_VER)// && (TRICK_VER < 10)
#include "interactions/contact/include/contact_params.hh"
#include "point_ground_interaction.hh"
#include "line_ground_interaction.hh"

#endif

#endif
