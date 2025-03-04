//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @file models/interactions/contact/include/contact_surface.hh
 * Vehicle surface model for contact
 */

/*****************************************************************************

 Purpose:
   ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

Library dependencies:
    ((../src/contact_surface.cc))


*****************************************************************************/

#ifndef JEOD_CONTACT_SURFACE_HH
#define JEOD_CONTACT_SURFACE_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/class_declarations.hh"
#include "utils/surface_model/include/interaction_surface.hh"

/* Model includes */
#include "contact_facet.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The contact specific interaction surface, for use with the surface model.
 */
class ContactSurface : public InteractionSurface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, ContactSurface)

public:
    ContactSurface();
    ~ContactSurface() override;
    ContactSurface & operator=(const ContactSurface &) = delete;
    ContactSurface(const ContactSurface &) = delete;

    /**
     * An array of pointers to contact interaction facets.
     */
    ContactFacet ** contact_facets{}; //!< trick_units(--)

    /**
     * Size of the contact_facets array
     */
    unsigned int facets_size{}; //!< trick_units(count)

    /**
     * Total Force due to contact, resulting from all plates combined.
     */
    double contact_force[3]{}; //!< trick_units(N)

    /**
     * Total Torque due to contact, resulting from all plates combined.
     */
    double contact_torque[3]{}; //!< trick_units(N/m)

    // Allocates the contact_facets array from the given size
    void allocate_array(unsigned int size) override;

    // Allocates the facet at the "index" value in contact_facets, using
    // the base Facet given by the pointer facet, and using the parameter
    // object pointed to by params pointer and using the
    // InteractionFacetFactory pointed to by factory.
    void allocate_interaction_facet(Facet * facet,
                                    InteractionFacetFactory * factory,
                                    FacetParams * params,
                                    unsigned int index) override;

    // collect the forces and torques from all the facets in this contact surface.
    virtual void collect_forces_torques();
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
