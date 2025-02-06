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
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/include/contact_facet.hh
 * Individual facets for use with contact interaction models
 */

/*****************************************************************************

 Purpose:
    ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

 Library dependencies:
    ((contact_facet.o))


*****************************************************************************/

#ifndef CONTACT_FACET_HH
#define CONTACT_FACET_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/interaction_facet.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod {

/**
 * An contact interaction specific facet for use in the surface model.
 */
class ContactFacet : public InteractionFacet {

   JEOD_MAKE_SIM_INTERFACES(ContactFacet)

public:
   /**
    * toggles this contact facet on and off, true=on false=off
    */
   bool active; //!< trick_units(--)

   /**
    * Stores the name of surface material that the facet is constructed of. This
    * information is used to deturmine the contact parameters used when pairs
    * are constructed.
    */
   ContactParams * surface_type; //!< trick_units(--)

   /**
    * DynBody associated with this facet for structural frame information.
    */
   DynBody* vehicle_body;  //!< trick_units(--)

   /**
    * maximum dimension of the contact facet for use in limiting pair in_contact calls
    */
   double max_dimension; //!< trick_units(m)

   /**
    * position of the facet in vehicle structural frame.
    */
   double position[3]; //!< trick_units(m)

   /**
    * normal of the facet relative to the vehicle structural frame.
    */
   double normal[3]; //!< trick_units(--)

   /**
    * Vehicle point for relstate calculations.
    */
   const BodyRefFrame* vehicle_point; //!< trick_units(--)

   // constructor
   ContactFacet ();

   // destructor
   virtual ~ContactFacet ();

   /**
    Create a vehicle point to track the state information of the contact
    facet.
    */
   void create_vehicle_point (void);

   /**
    Calculate the max dimension of the facet for range limit determination.
    */
   virtual void set_max_dimension(
      void)
   = 0;

   // Name access.
   const char * get_name (void) const;

   /**
    Calculate the torque acting on the facet in the vehicle structural frame.
    */
   virtual void calculate_torque(
      double *tmp_force) // In: N force from one contact interaction.
   = 0;

   /**
    Overloaded functions that create a ContactPair and pass the address
    of it to the Contact class for addition to the list of pairs.
    This function is called to create a pair that only contains a subject.
    */
   virtual ContactPair * create_pair( /* Return: -- ContactPair that was created */
      void)
   = 0;

   /**
    Overloaded functions that creates a ContactPair and pass the address
    of it to the Contact class for addition to the list of pairs.
    This function is called when a subject and target are known.
    */
   virtual ContactPair * create_pair ( /* Return: -- ContactPair that was created */
      ContactFacet * target, /* InOut: -- target ContactFacet */
      Contact * contact) /* In: -- Contact object used to find the pair interaction */
   = 0;

private:
   /* Operator = and copy constructor hidden from use by being private */

   ContactFacet& operator = (const ContactFacet& rhs);
   ContactFacet (const ContactFacet& rhs);

};

/**
 * Accessor for name.
 * @return Point name
 */
inline const char *
ContactFacet::get_name (
   void)
const
{
   return base_facet->name.c_str();
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
