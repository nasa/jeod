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
 * @file models/interactions/contact/include/line_point_contact_pair.hh
 * Class for a pair of a line contact facet and a point contact facet for use
 * with contact interaction model
 */

/*****************************************************************************

 Purpose:
 ()

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((../src/line_point_contact_pair.cc))



 *****************************************************************************/

#ifndef LINE_POINT_CONTACT_PAIR_HH
#define LINE_POINT_CONTACT_PAIR_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "contact_pair.hh"

//! Namespace jeod
namespace jeod
{

/**
 * An point to point contact pair for use in the contact model.
 */
class LinePointContactPair : public ContactPair
{
    JEOD_MAKE_SIM_INTERFACES(jeod, LinePointContactPair)

public:
    /**
     * pointer to the contact facet that is the subject of the associated relative states.
     */
    LineContactFacet * line_subject{}; //!< trick_units(--)

    /**
     * pointer to the contact facet that is the target of the associated relative states.
     */
    PointContactFacet * point_target{}; //!< trick_units(--)

    LinePointContactPair() = default;
    ~LinePointContactPair() override = default;
    LinePointContactPair & operator=(const LineContactPair &) = delete;
    LinePointContactPair(const LineContactPair &) = delete;

    // function that determines of a if the facets are in contact.
    void in_contact() override;

    // initialize the contact pair, creating relstates if possible.
    void initialize_pair(ContactFacet * subject_facet, ContactFacet * target_facet) override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
