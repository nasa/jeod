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
 * @file models/interactions/contact/include/point_contact_facet.hh
 * The contact facet based on the distance to a single point, specifically
 * the vehicle point.  In effect this represents a sphere.
 */

/*****************************************************************************

 Purpose:
    ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

Library dependencies:
    ((point_contact_facet.o))

 

*****************************************************************************/

#ifndef POINT_CONTACT_FACET_HH
#define POINT_CONTACT_FACET_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"
#include "dynamics/derived_state/include/relative_derived_state.hh"

/* Model includes */
#include "class_declarations.hh"
#include "contact_facet.hh"

//! Namespace jeod
namespace jeod {

/**
 * The contact facet based on the distance to a single point, specifically
 * the vehicle point.  In effect this represents a sphere.
 */
class PointContactFacet : public ContactFacet {

   JEOD_MAKE_SIM_INTERFACES (PointContactFacet)

public:
   /**
    * radius from the point at which contact takes place.
    */
   double radius; //!< trick_units(m)

   /**
    * Contact point given in facet vehicle point frame, representing point
    * on the point on the surface of a sphere of radius "radius" where
    * contact has occured.
    */
   double contact_point[3]; //!< trick_units(m)

   // constructor
   PointContactFacet ();

   // destructor
   virtual ~PointContactFacet ();

   /*
    Overloaded functions to create a contact pair of the appropriate type and return
    a pointer too the pair to the Contact class.
    */
   virtual ContactPair * create_pair();
   virtual ContactPair * create_pair(ContactFacet * target, Contact *contact);

   // calculate the max dimension of the facet for range limit determination
   virtual void set_max_dimension();

   // calculate the contact point based on the relstate and radius.
   virtual void calculate_contact_point (double nvec[3]);

   // calculate the torque acting on the facet in the vehicle structural frame.
   virtual void calculate_torque(double *tmp_force);

private:
   // Operator = and copy constructor locked from use by being made private
   PointContactFacet& operator = (const PointContactFacet & rhs);
   PointContactFacet (const PointContactFacet & rhs);

};

} // End JEOD namespace

#ifdef TRICK_VER
#include "point_contact_pair.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
