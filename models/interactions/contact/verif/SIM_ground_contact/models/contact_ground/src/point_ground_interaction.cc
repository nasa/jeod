/*****************************************************************************

 Purpose:
 (PointGroundInteraction class for use with contact interaction model)

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((point_ground_interaction.o))

 

 *****************************************************************************/

/* JEOD includes */
#include "utils/math/include/vector3.hh"
#include "interactions/contact/include/point_contact_facet.hh"
#include "interactions/contact/include/pair_interaction.hh"
#include "interactions/contact/include/contact_utils.hh"
#include "dynamics/dyn_body/include/body_ref_frame.hh"

/* Model includes */
#include "../include/point_ground_interaction.hh"
#include "../include/ground_facet.hh"

//! Namespace jeod
namespace jeod {

/*******************************************************************************
 Function: PointGroundInteraction
 Purpose: (Default Constructor)
 *******************************************************************************/
PointGroundInteraction::PointGroundInteraction (
   void)
: // Return: -- None
subject_point (NULL)
{

}


/*******************************************************************************
 Function: ~PointGroundInteraction
 Purpose: (Destructor)
 *******************************************************************************/
PointGroundInteraction::~PointGroundInteraction ( // Return: -- void
                           void)
{

}

/*******************************************************************************
 Function: in_contact
 Purpose: (deturmine if the subject facet is in contact with the ground facet)
 *******************************************************************************/
void PointGroundInteraction::in_contact ( // Return: -- void
   void)
{
   double facet_pos[3]; /* M vehicle point position in planet frame. */
   double rel_state[3]; /* M rel_state vector between facet and a point on the ground */
   double vec[3], nvec[3];
   double ground_point[3], ground_mag;
   double rel_velocity[3], subject_mag;


   // find altitude at the vehicle point
   Vector3::sum(subject_point->vehicle_body->structure.state.trans.position,
                subject_point->vehicle_point->state.trans.position,
                vec);
   // rotate from inertial to pfix
   Vector3::transform(terrain->planet->pfix.state.rot.T_parent_this,
                      vec,
                      facet_pos);

   // get lat long in pfix and use to calculate altitude
   point.update_from_cart (facet_pos);
   terrain->find_altitude(&point, ground_normal);

   // update returned terrain value with the ground facet offset.
   point.ellip_coords.altitude += ground->alt_offset;
   point.update_from_ellip (point.ellip_coords);

   // convert ground point to vehicle point frame
   Vector3::transform_transpose (terrain->planet->pfix.state.rot.T_parent_this,
      point.cart_coords, vec);
   Vector3::transform (subject_point->vehicle_point->state.rot.T_parent_this,
                                 vec, ground_point);

   // use center of planet as direction vector in calculation of closest point.
   subject_point->calculate_contact_point(ground_point);

   // convert facet position to vehicle point frame
   Vector3::transform (subject_point->vehicle_point->state.rot.T_parent_this,
      subject_point->vehicle_point->state.trans.position, facet_pos);

   // find total distance between contact point and planet origin
   Vector3::sum(subject_point->contact_point, facet_pos, rel_state);

   subject_mag = Vector3::vmag(rel_state);
   ground_mag = Vector3::vmag(ground_point);

   // compare length to ground with subject contact point
   if (subject_mag < ground_mag) {
      // find the vector that characterizes the interpenetration
      Vector3::diff(ground_point, rel_state, vec);


 // TODO: worry about planetary rotation
      // find the relative velocity of the target in subject frame
      Vector3::cross (subject_point->vehicle_point->state.rot.ang_vel_this,
		      subject_point->contact_point,
                      rel_velocity);
      Vector3::transform (subject_point->vehicle_point->state.rot.T_parent_this,
         subject_point->vehicle_point->state.trans.velocity, nvec);
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
