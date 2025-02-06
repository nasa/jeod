/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/point_contact_pair.cc
 * ContactPair class for use with contact interaction model
 */

/*****************************************************************************

 Purpose:
 ()

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((point_contact_pair.cc))



 *****************************************************************************/

/* JEOD includes */
#include "utils/named_item/include/named_item.hh"

/* Model includes */
#include "../include/contact_utils.hh"
#include "../include/pair_interaction.hh"
#include "../include/point_contact_facet.hh"
#include "../include/point_contact_pair.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Determine if contact has occurred between the facets of the pair.
 */
void PointContactPair::in_contact()
{
    double subject_contact_point[3];
    double target_contact_point[3];
    double vec[3], nvec[3];
    double target_mag;

    // find direction vector to target from relstate
    Vector3::normalize(rel_state.rel_state.trans.position, vec);
    // pass direction vector to the subject so it can calculate the closest point.
    point_subject->calculate_contact_point(vec);

    // rotate direction vector to target frame
    Vector3::transform(rel_state.rel_state.rot.T_parent_this, rel_state.rel_state.trans.position, nvec);
    Vector3::negate(nvec, nvec);
    Vector3::normalize(nvec, vec);
    point_target->calculate_contact_point(vec);

    // get the contact points calculated above
    Vector3::copy(point_subject->contact_point, subject_contact_point);
    Vector3::copy(point_target->contact_point, target_contact_point);

    // convert target point into subject frame through rotation vector addition
    Vector3::transform_transpose(rel_state.rel_state.rot.T_parent_this, target_contact_point, vec);

    Vector3::sum(rel_state.rel_state.trans.position, vec, target_contact_point);
    // find the distance to the target_contact_point
    target_mag = Vector3::vmag(target_contact_point);

    // compare length to target point with subject radius
    if(point_subject->radius > target_mag)
    {
        double rel_velocity[3];
        // find the vector that characterizes the interpenetration
        Vector3::diff(target_contact_point, subject_contact_point, vec);
        // find the relative velocity of the target in subject frame
        Vector3::cross(rel_state.rel_state.rot.ang_vel_this, subject_contact_point, rel_velocity);
        Vector3::diff(rel_velocity, rel_state.rel_state.trans.velocity, rel_velocity);
        // calculate the forces on the facets
        interaction->calculate_forces(point_subject, point_target, &rel_state, vec, rel_velocity);
    }
}

/**
 * Initialize the contact pair by setting the subject, target, and creating
 * the relstate if possible.
 * \param[in,out] subject_facet subject ContactFacet
 * \param[in,out] target_facet target ContactFacet
 */
void PointContactPair::initialize_pair(ContactFacet * subject_facet, ContactFacet * target_facet)
{
    point_subject = dynamic_cast<PointContactFacet *>(subject_facet);
    if(target_facet != nullptr)
    {
        point_target = dynamic_cast<PointContactFacet *>(target_facet);
        rel_state.name = NamedItem::construct_name(point_subject->base_facet->name, point_target->base_facet->name);

        rel_state.direction_sense = RelativeDerivedState::ComputeTargetStateinSubject;

        rel_state.subject_frame_name = point_subject->vehicle_point->get_name();
        rel_state.target_frame_name = point_target->vehicle_point->get_name();
    }
    else
    {
        point_target = nullptr;
    }
    subject = point_subject;
    target = point_target;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
