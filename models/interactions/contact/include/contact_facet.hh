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
    ((../src/contact_facet.cc))


*****************************************************************************/

#ifndef CONTACT_FACET_HH
#define CONTACT_FACET_HH

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/interaction_facet.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * An contact interaction specific facet for use in the surface model.
 */
class ContactFacet : public InteractionFacet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, ContactFacet)

public:
    /**
     * toggles this contact facet on and off, true=on false=off
     */
    bool active{true}; //!< trick_units(--)

    /**
     * Stores the name of surface material that the facet is constructed of. This
     * information is used to deturmine the contact parameters used when pairs
     * are constructed.
     */
    ContactParams * surface_type{}; //!< trick_units(--)

    /**
     * DynBody associated with this facet for structural frame information.
     */
    DynBody * vehicle_body{}; //!< trick_units(--)

    /**
     * maximum dimension of the contact facet for use in limiting pair in_contact calls
     */
    double max_dimension{}; //!< trick_units(m)

    /**
     * position of the facet in vehicle structural frame.
     */
    double position[3]{}; //!< trick_units(m)

    /**
     * normal of the facet relative to the vehicle structural frame.
     */
    double normal[3]{}; //!< trick_units(--)

    /**
     * Vehicle point for relstate calculations.
     */
    const BodyRefFrame * vehicle_point{}; //!< trick_units(--)

    ContactFacet() = default;
    ~ContactFacet() override = default;
    ContactFacet & operator=(const ContactFacet &) = delete;
    ContactFacet(const ContactFacet &) = delete;

    /**
     Create a vehicle point to track the state information of the contact
     facet.
     */
    void create_vehicle_point();

    /**
     Calculate the max dimension of the facet for range limit determination.
     */
    virtual void set_max_dimension() = 0;

    // Name access.
    std::string get_name() const;

    /**
     Calculate the torque acting on the facet in the vehicle structural frame.
     In: N force from one contact interaction.
     */
    virtual void calculate_torque(double * tmp_force) = 0;

    /**
     Overloaded functions that create a ContactPair and pass the address
     of it to the Contact class for addition to the list of pairs.
     This function is called to create a pair that only contains a subject.
     Return: -- ContactPair that was created
     */
    virtual ContactPair * create_pair() = 0;

    /**
     Overloaded functions that creates a ContactPair and pass the address
     of it to the Contact class for addition to the list of pairs.
     This function is called when a subject and target are known.
     Return: -- ContactPair that was created
     InOut: -- target ContactFacet
     In: -- Contact object used to find the pair interaction
     */
    virtual ContactPair * create_pair(ContactFacet * target, Contact * contact) = 0;
};

/**
 * Accessor for name.
 * @return Point name
 */
inline std::string ContactFacet::get_name() const
{
    return base_facet->name;
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
