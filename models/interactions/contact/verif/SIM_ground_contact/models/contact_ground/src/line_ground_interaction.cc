/*****************************************************************************

 Purpose:
 (LineGroundInteraction class for use with contact interaction model)

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((line_ground_interaction.o))

 

 *****************************************************************************/

/* JEOD includes */
#include "utils/math/include/vector3.hh"
#include "interactions/contact/include/line_contact_facet.hh"
#include "interactions/contact/include/pair_interaction.hh"
#include "interactions/contact/include/contact_utils.hh"
#include "dynamics/dyn_body/include/body_ref_frame.hh"

/* Model includes */
#include "../include/line_ground_interaction.hh"
#include "../include/ground_facet.hh"

//! Namespace jeod
namespace jeod {

/*******************************************************************************
 Function: LineGroundInteraction
 Purpose: (Default Constructor)
 *******************************************************************************/
LineGroundInteraction::LineGroundInteraction (
   void)
: // Return: -- None
subject_line (NULL)
{

}


/*******************************************************************************
 Function: ~LineGroundInteraction
 Purpose: (Destructor)
 *******************************************************************************/
LineGroundInteraction::~LineGroundInteraction ( // Return: -- void
   void)
{

}

/*******************************************************************************
 Function: in_contact
 Purpose: (deturmine if the subject facet is in contact with the ground facet)
 *******************************************************************************/
void
LineGroundInteraction::in_contact ( // Return: -- void
   void)
{
   double rel_state[3]; /* M rel_state vector between facet and a point on the ground */
   double sub_p1[3], sub_p2[3]; // subject line segment end points
   double vec[3], nvec[3];
   double ground_point[3], facet_pos[3];
   double ground_mag, subject_mag;
   double rel_velocity[3];

   // initialize the end points of the line segments
   Vector3::initialize(sub_p1);
   Vector3::initialize(sub_p2);

   // set end points of line segments assuming cylinder aligned with x-axis
   sub_p1[0] = - subject_line->length / 2.0;
   sub_p2[0] = subject_line->length / 2.0;

   // find altitude at the vehicle point
   Vector3::sum(subject_line->vehicle_body->structure.state.trans.position,
                subject_line->vehicle_point->state.trans.position,
                vec);
   Vector3::transform(terrain->planet->pfix.state.rot.T_parent_this,
      vec,
      facet_pos);

   point.update_from_cart (facet_pos);

   terrain->find_altitude(&point, ground_normal);

   // update returned terrain value with the ground facet offset.
   point.ellip_coords.altitude += ground->alt_offset;
   point.update_from_ellip (point.ellip_coords);

   // convert ground point to vehicle point frame for use as the closest point to the line.
   Vector3::transform_transpose (terrain->planet->pfix.state.rot.T_parent_this,
      point.cart_coords, vec);
   Vector3::transform (subject_line->vehicle_point->state.rot.T_parent_this,
                                 vec, ground_point);
   // find the closest point on the two line segments.
   ContactUtils::dist_line_segments(
      sub_p1, sub_p2,
      ground_point, ground_point,
      subject_line->contact_point, ground_point);

   // find the contact point on the surface of the subject
   subject_line->calculate_contact_point(ground_point);

   // convert facet position to vehicle point frame
   Vector3::transform (subject_line->vehicle_point->state.rot.T_parent_this,
      subject_line->vehicle_point->state.trans.position, facet_pos);

   // find total distance between contact point and planet origin
   Vector3::sum(subject_line->contact_point, facet_pos, rel_state);

   subject_mag = Vector3::vmag(rel_state);
   ground_mag = Vector3::vmag(ground_point);

   // compare length to ground with subject contact point
   if (subject_mag < ground_mag) {
      // find the vector that characterizes the interpenetration
      Vector3::diff(ground_point, rel_state, vec);

// TODO: worry about planetary rotation
      // find the relative velocity of the target in subject frame
      Vector3::cross (subject_line->vehicle_point->state.rot.ang_vel_this,
                      subject_line->contact_point,
                      rel_velocity);
      Vector3::transform (subject_line->vehicle_point->state.rot.T_parent_this,
         subject_line->vehicle_point->state.trans.velocity, nvec);
      Vector3::sum (rel_velocity,
                    nvec,
                    rel_velocity);

      // calculate the forces on the facets
      interaction->calculate_forces(subject, NULL, NULL, vec, rel_velocity);

      contact = true;
      velocity_mag = Vector3::vmag(rel_velocity);
      friction_mag = interaction->friction_mag;
   }
   else
   {
      contact = false;
      velocity_mag = 0.0;
      friction_mag = 0.0;
   }

   return;
}
} // End JEOD namespace
