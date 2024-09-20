/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/spring_pair_interaction.cc
 * spring pair interaction for use in the contact model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((spring_pair_interaction.cc))


*******************************************************************************/

/* System includes */
#include <cmath>

/* JEOD includes */
#include "dynamics/derived_state/include/relative_derived_state.hh"
#include "dynamics/dyn_body/include/body_ref_frame.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/math/include/vector3.hh"

/* Model includes */
#include "../include/contact_facet.hh"
#include "../include/spring_pair_interaction.hh"

//! Namespace jeod
namespace jeod
{

/**
 * force calculation for a simple spring based contact
 * dynamics model, takes in geometry information from the appropriate
 * ContactFacet::calculate_forces but doesn't know about specific type of
 * ContactFacet
 * \param[in,out] subject subject frame of the relative state
 * \param[in,out] target target frame of the relative state
 * \param[in] rel_state relative state between subject and target in subject frame
 * \param[in] penetration_vector vector that characterises the interpenetration of the subject and the target
 * \param[in] rel_velocity relative velocity of the subject and the target in the subject frame
 */
void SpringPairInteraction::calculate_forces(ContactFacet * subject,
                                             ContactFacet * target,
                                             RelativeDerivedState * rel_state,
                                             double * penetration_vector,
                                             double * rel_velocity)
{
    double friction_vec[3];             /* Friction directional vector */
    double damping_force[3];            /* Damping force */
    double friction_force[3];           /* Friction force */
    double nvec[3], vec[3];             /* temp vectors */
    double mag, mag_force, damping_mag; /* vector magnitudes */
    double force[3], tmp_force[3];      /* forces */

    // normalize penetration vector and avoid really small values
    Vector3::zero_small(1.0E-10, penetration_vector);
    Vector3::normalize(penetration_vector, nvec);

    /* Spring Constant Force */
    // scale penetract vector with the spring constant
    Vector3::scale(penetration_vector, this->spring_k, force);

    /* Damping Force */
    // mag of velocity in penetration direction
    mag = Vector3::dot(rel_velocity, nvec);
    // calculate magnitude of damping force
    damping_mag = mag * this->damping_b;
    // scale nvec with force magnitue to get damping force
    Vector3::scale(nvec, -damping_mag, damping_force);

    // add damping force to total force on facet
    Vector3::sum(force, damping_force, force);

    /* Friction Force */
    Vector3::normalize(rel_velocity, friction_vec);
    // cross velocity with penetration vector
    Vector3::cross(friction_vec, nvec, vec);
    // cross penetration vector with result to get vector along subject surface
    Vector3::cross(nvec, vec, friction_vec);

    // calcualte the magnitude of the spring force
    mag_force = Vector3::vmag(force);
    mag = mag_force * this->mu;
    // scale surface vector with magnitude
    Vector3::scale(friction_vec, -mag, friction_force);
    Vector3::sum(force, friction_force, force);
    friction_mag = Vector3::vmag(friction_force);

    /* add Force and Torque to Subject Facet */
    // rotate force into vehicle structure frame
    Vector3::transform_transpose(subject->vehicle_point->state.rot.T_parent_this, force, vec);
    Vector3::transform(subject->vehicle_body->structure.state.rot.T_parent_this, vec, tmp_force);
    // calculate torque and add to facet total
    subject->calculate_torque(tmp_force);
    // add force to facet total
    Vector3::sum(subject->force, tmp_force, subject->force);

    /* add Force and Torque to Target Facet */
    if(target != nullptr)
    {
        // negate force to make it equal and opposite to force on subject
        Vector3::negate(force, force);
        // rotate force to target frame
        Vector3::transform(rel_state->rel_state.rot.T_parent_this, force, nvec);
        // rotate force into vehicle structure frame
        Vector3::transform_transpose(target->vehicle_point->state.rot.T_parent_this, nvec, vec);
        Vector3::transform(target->vehicle_body->structure.state.rot.T_parent_this, vec, tmp_force);
        // calculate torque and add to facet total
        target->calculate_torque(tmp_force);
        // add force to facet total
        Vector3::sum(target->force, tmp_force, target->force);
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
