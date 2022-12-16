/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/line_contact_pair.cc
 * LineContactPair class for use with contact interaction model
 */

/*****************************************************************************

 Purpose:
 ()

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((line_contact_pair.o))

 

 *****************************************************************************/

/* JEOD includes */
#include "dynamics/mass/include/mass.hh"
#include "utils/named_item/include/named_item.hh"

/* Model includes */
#include "../include/line_contact_pair.hh"
#include "../include/line_contact_facet.hh"
#include "../include/pair_interaction.hh"
#include "../include/contact_utils.hh"

//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */
LineContactPair::LineContactPair (
   void)
: // Return: -- None
line_subject (NULL),
line_target (NULL)
{

}


/**
 * Destructor
 */
LineContactPair::~LineContactPair (
   void)
{

}

/**
 * Determine if contact has occurred between the facets of the pair.
 */
void
LineContactPair::in_contact(
   void)
{
   double subject_contact_point[3]; // subject contact point
   double target_contact_point[3];  // target contact point
   double sub_p1[3], sub_p2[3]; // subject line segment end points
   double tar_p3[3], tar_p4[3]; // target line segment end points
   double vec[3];  // temp vectors
   double rel_velocity[3];  // relative velocity in subject frame
   double subject_mag, target_mag;  // distace vector magnitudes

   // initialize the end points of the line segments
   Vector3::initialize(sub_p1);
   Vector3::initialize(sub_p2);
   Vector3::initialize(tar_p3);
   Vector3::initialize(tar_p4);

   // set end points of line segments assuming cylinder aligned with x-axis
   sub_p1[0] = - line_subject->length / 2.0;
   sub_p2[0] = line_subject->length / 2.0;
   tar_p3[0] = - line_target->length / 2.0;
   tar_p4[0] = line_target->length / 2.0;

   // convert end points into subject frame through rotation vector addition
   Vector3::transform_transpose (
      rel_state.rel_state.rot.T_parent_this,
      tar_p3, vec);
   Vector3::sum(rel_state.rel_state.trans.position, vec, tar_p3);
   Vector3::transform_transpose (
      rel_state.rel_state.rot.T_parent_this,
      tar_p4, vec);
   Vector3::sum(rel_state.rel_state.trans.position, vec, tar_p4);

   // find the closest point on the two line segments.
   ContactUtils::dist_line_segments(
      sub_p1, sub_p2,
      tar_p3, tar_p4,
      subject_contact_point, target_contact_point);

   // put contact point calculated above into the subject facet
   Vector3::copy(subject_contact_point, line_subject->contact_point);

   // find vector between the two close points
   Vector3::diff(target_contact_point, subject_contact_point, vec);
   //Vector3::normalize(vec, nvec);

   // find the contact point on the surface of the subject
   line_subject->calculate_contact_point(vec);

   // convert subject and target vectors into target frame
   // subject line closest point
   Vector3::diff(subject_contact_point, rel_state.rel_state.trans.position, vec);
   Vector3::transform (rel_state.rel_state.rot.T_parent_this,
                       vec, subject_contact_point);
   // target line closest point
   Vector3::diff(target_contact_point, rel_state.rel_state.trans.position, vec);
   Vector3::transform (rel_state.rel_state.rot.T_parent_this,
                       vec, target_contact_point);

   // put contact point calculated above into the target facet
   Vector3::copy(target_contact_point, line_target->contact_point);

   // find vector between the two close points
   Vector3::diff(subject_contact_point, target_contact_point, vec);
   //Vector3::normalize(vec, nvec);

   // find the contact point on the surface of the target
   line_target->calculate_contact_point(vec);

   // get the contact points back from the facets
   Vector3::copy(line_subject->contact_point, subject_contact_point);
   Vector3::copy(line_target->contact_point, target_contact_point);

   // convert target point into subject frame through rotation and vector addition
   Vector3::transform_transpose (rel_state.rel_state.rot.T_parent_this,
                       target_contact_point, vec);
   Vector3::sum(rel_state.rel_state.trans.position, vec, target_contact_point);

   // find the distances to the contact points
   subject_mag = Vector3::vmag(subject_contact_point);
   target_mag = Vector3::vmag(target_contact_point);

   // compare length to target point with subject radius
   if (subject_mag > target_mag) {
      // find the vector that characterizes the interpenetration
      Vector3::diff(target_contact_point, subject_contact_point, vec);
      // find the relative velocity of the target in subject frame
      Vector3::cross (rel_state.rel_state.rot.ang_vel_this, subject_contact_point, rel_velocity);
      Vector3::diff (rel_velocity, rel_state.rel_state.trans.velocity, rel_velocity);
      // calculate the forces on the facets
      interaction->calculate_forces(line_subject, line_target, &rel_state, vec, rel_velocity);
   }

   return;
}

/**
 * Initialize the contact pair by setting the subject, target, and creating
 * the relstate if possible.
 * \param[in,out] subject_facet subject ContactFacet
 * \param[in,out] target_facet target ContactFacet
 */
void LineContactPair::initialize_pair (
   ContactFacet *subject_facet,
   ContactFacet *target_facet)
{
   line_subject = dynamic_cast<LineContactFacet*> (subject_facet);
   if (target_facet != NULL) {
      line_target = dynamic_cast<LineContactFacet*> (target_facet);
      rel_state.name = NamedItem::construct_name_string (
         line_subject->base_facet->name.c_str(), line_target->base_facet->name);

      rel_state.direction_sense =
         RelativeDerivedState::ComputeTargetStateinSubject;

      ContactUtils::copy_const_char_to_char (line_subject->vehicle_point->get_name (),
                                             &rel_state.subject_frame_name);
      ContactUtils::copy_const_char_to_char (line_target->vehicle_point->get_name (),
                                             &rel_state.target_frame_name);
   }
   else {
      line_target = NULL;
   }

   subject = line_subject;
   target = line_target;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
