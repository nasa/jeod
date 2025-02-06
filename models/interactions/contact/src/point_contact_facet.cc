/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/point_contact_facet.cc
 * Define PointContactFacet functions
 */

/*******************************************************************************
Purpose:
  ()

Library Dependencies:
  ((contact_messages.cc)
   (point_contact_facet.cc)
   (point_contact_pair.cc))


*******************************************************************************/

/* System includes */
#include <cmath>
#include <cstring>

/* JEOD includes */
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/numerical.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"

/* Model includes  */
#include "../include/contact.hh"
#include "../include/contact_messages.hh"
#include "../include/contact_params.hh"
#include "../include/contact_utils.hh"
#include "../include/point_contact_facet.hh"
#include "../include/point_contact_pair.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default constructor
 */
PointContactFacet::PointContactFacet()
{
    JEOD_REGISTER_CLASS(PointContactPair);
}

/**
 * Overloaded functions that create a ContactPair and pass the address
 * of it to the Contact class for addition to the list of pairs.
 * This function is called to create a pair that only contains a subject.
 * @return ContactPair that was created
 */
ContactPair * PointContactFacet::create_pair()
{
    PointContactPair * contact_pair;
    // Allocate a ContactPair instance
    contact_pair = JEOD_ALLOC_CLASS_OBJECT(PointContactPair, ());
    // set the subject and target of the new pair
    contact_pair->initialize_pair(this, nullptr);

    return contact_pair;
}

/**
 * Overloaded functions that create a ContactPair and pass the address
 * of it to the Contact class for addition to the list of pairs.
 * This function is called when a subject and target are known.
 * @return ContactPair that was created
 * \param[in,out] target target ContactFacet
 * \param[in] contact Contact object used to find the pair interaction
 */
ContactPair * PointContactFacet::create_pair(ContactFacet * target, Contact * contact)
{
    PointContactFacet * temp_ptr;

    // attempt to cast the target to a PointContactFacet
    temp_ptr = dynamic_cast<PointContactFacet *>(target);
    if(temp_ptr == nullptr)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             ContactMessages::initialization_warns,
                             "The target ContactFacet passed into PointContactFacet::create_pair was "
                             "not of type PointContactFacet as required.");

        return nullptr;
    }

    PointContactPair * contact_pair;
    // Allocate a ContactPair instance
    contact_pair = JEOD_ALLOC_CLASS_OBJECT(PointContactPair, ());

    contact_pair->interaction = contact->find_interaction(surface_type, target->surface_type);
    if(contact_pair->interaction == nullptr)
    {
        return nullptr;
    }
    // set the subject and target of the new pair
    contact_pair->initialize_pair(this, temp_ptr);

    contact_pair->interaction_distance = (max_dimension + temp_ptr->max_dimension) * contact->contact_limit_factor;

    return contact_pair;
}

/**
 * Use the relstate and radius of contact to calculate a contact point
 * on this facet.
 * \param[in] nvec direction vector between the two facets
 */
void PointContactFacet::calculate_contact_point(double nvec[3])
{
    double vec[3];

    Vector3::normalize(nvec, vec);
    // Scale the normal direction by the radius of interaction for the point contact facet.
    Vector3::scale(vec, radius, contact_point);
}

/**
 * calculate the max dimension of the facet for range limit determination.
 */
void PointContactFacet::set_max_dimension()
{
    max_dimension = radius;
}

/**
 * Calculate the torque generated on the vehicle by the facet.  Assumes
 * that the force is in the vehicle structural frame, but that close_point is not.
 * \param[in] tmp_force force from one contact interaction.\n Units: N
 */
void PointContactFacet::calculate_torque(double * tmp_force)
{
    double cm[3];
    double vec[3];
    double tmp_torque[3];

    // find the vector from vehicle point to center of mass
    Vector3::diff(vehicle_point->mass_point->position, vehicle_body->mass.composite_properties.position, cm);

    // rotate facet contact point to vehicle structure frame
    Vector3::transform_transpose(vehicle_point->state.rot.T_parent_this, contact_point, vec);
    Vector3::transform(vehicle_body->structure.state.rot.T_parent_this, vec, tmp_torque);
    // find vector from cm to contact point
    Vector3::sum(tmp_torque, cm, cm);
    // cross cm vector with the rotated force to get the torque
    Vector3::cross(cm, tmp_force, tmp_torque);
    Vector3::sum(torque, tmp_torque, torque);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
