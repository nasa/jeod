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
/*****************************************************************************

 Purpose:
 (Base class for the interaction between the ground and contact facet)

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((../src/ground_interaction.cc))

 

 *****************************************************************************/

#ifndef GROUND_INTERACTION_HH
#define GROUND_INTERACTION_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"
#include "interactions/contact/include/class_declarations.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"
#include "interactions/contact/include/contact_facet.hh"

/* Rover includes */
#include "../../terrain/include/terrain.hh"



//! Namespace jeod
namespace jeod {

// Class declarations
class Terrain;
class GroundFacet;

/*
 Purpose:
 (An base ground interaction class for use in the contact model.)
 */
class GroundInteraction {

   JEOD_MAKE_SIM_INTERFACES(GroundInteraction)

public:
   ContactFacet *subject; /* trick_units(--) @n
      contact facet that can interact with the ground */

   GroundFacet *ground; /* trick_units(--) @n
      ground contact facet */

   PairInteraction *interaction; /* trick_units(--) @n
      Parameters that define the force calculation function between the contact
      facet and the ground */

   double interaction_distance; /* trick_units(m) @n
      rel_state distance at which in_contact should be called. */

   bool contact; /* trick_units(--) @n
      the subject is touching the ground. */

   double friction_mag;  /* trick_units(N) @n
      magnitude of the friction force on the contact surface. */

   double velocity_mag; /* trick_units(m/s) @n
      magnitude of the relative velocity between the ground and facet. */

   Terrain * terrain; /* trick_units(--) @n
      terrain object associated with this interaction. */

   PlanetFixedPosition point; /* trick_units(--) @n
      planet fixed position used to store ground location. */

   double ground_normal[3]; /* trick_units(--) @n
      ground normal from last in contact calculation. */

   // constructor
   GroundInteraction();

   // destructor
   virtual ~GroundInteraction();

   virtual void initialize(Terrain *terrain_in);

   // test whether facet is in range of the section of ground. */
   bool in_range();

   // check to make sure the contact facet is active.
   bool is_active();

   // check to make sure the contact facet is active.
   ContactFacet * get_subject();

   /*
    Virtual funtion to determine if the contact facet is touching the ground.
    */
   virtual void in_contact() = 0;

protected:

private:

   /* Operator = and copy constructor hidden from use by being private */
   GroundInteraction& operator = (const GroundInteraction & rhs);
   GroundInteraction (const GroundInteraction & rhs);

};

} // End JEOD namespace

#if (defined TRICK_VER)// && (TRICK_VER < 10)
#include "interactions/contact/include/pair_interaction.hh"
#include "ground_facet.hh"
#endif


#endif
