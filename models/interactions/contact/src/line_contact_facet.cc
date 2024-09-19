/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/line_contact_facet.cc
 * Define LineContactFacet functions
 */

/*******************************************************************************
Purpose:
  ()

Library Dependencies:
  ((contact_messages.cc)
   (line_contact_facet.cc)
   (line_contact_pair.cc)
   (line_point_contact_pair.cc))


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
#include "../include/line_contact_facet.hh"
#include "../include/line_contact_pair.hh"
#include "../include/line_point_contact_pair.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default constructor
 */
LineContactFacet::LineContactFacet()
{
    JEOD_REGISTER_CLASS(LineContactPair);
    JEOD_REGISTER_CLASS(LinePointContactPair);
}

/**
 * Overloaded functions that create a ContactPair and pass the address
 * of it to the Contact class for addition to the list of pairs.
 * This function is called to create a pair that only contains a subject.
 * @return ContactPair that was created
 */
ContactPair * LineContactFacet::create_pair()
{
    LineContactPair * contact_pair;
    // Allocate a ContactPair instance
    contact_pair = JEOD_ALLOC_CLASS_OBJECT(LineContactPair, ());
    // set the subject and target of the new pair
    contact_pair->initialize_pair(this, nullptr);

    return contact_pair;
}

/**
 * Overloaded functions that creates a ContactPair and pass the address
 * of it to the Contact class for addition to the list of pairs.
 * This function is called when a subject and target are known.
 * @return ContactPair that was created
 * \param[in,out] target target ContactFacet
 * \param[in] contact Contact object used to find the pair interaction
 */
ContactPair * LineContactFacet::create_pair(ContactFacet * target, Contact * contact)
{
    LineContactFacet * temp_line;
    PointContactFacet * temp_point;

    // attempt to cast the target to a LineContactFacet or PointContactFacet
    temp_line = dynamic_cast<LineContactFacet *>(target);
    temp_point = dynamic_cast<PointContactFacet *>(target);
    if(temp_line == nullptr && temp_point == nullptr)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             ContactMessages::initialization_warns,
                             "The target ContactFacet passed into LineContactFacet::create_pair was "
                             "not of type LineContactFacet or PointContactFAcet as required.");

        return nullptr;
    }
    if(temp_line == nullptr)
    {
        LinePointContactPair * contact_pair;
        // Allocate a ContactPair instance
        contact_pair = JEOD_ALLOC_CLASS_OBJECT(LinePointContactPair, ());

        contact_pair->interaction = contact->find_interaction(surface_type, target->surface_type);
        if(contact_pair->interaction == nullptr)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 ContactMessages::initialization_warns,
                                 "No interaction type could be found for the surface types of these ContactFacets.");

            JEOD_DELETE_OBJECT(contact_pair);
            return nullptr;
        }

        // set the subject and target of the new pair
        contact_pair->initialize_pair(this, temp_point);

        contact_pair->interaction_distance = (max_dimension + temp_point->max_dimension) *
                                             contact->contact_limit_factor;

        return contact_pair;
    }
    else
    {
        LineContactPair * contact_pair;
        // Allocate a ContactPair instance
        contact_pair = JEOD_ALLOC_CLASS_OBJECT(LineContactPair, ());

        contact_pair->interaction = contact->find_interaction(surface_type, target->surface_type);
        if(contact_pair->interaction == nullptr)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 ContactMessages::initialization_warns,
                                 "No interaction type could be found for the surface types of these ContactFacets.");

            JEOD_DELETE_OBJECT(contact_pair);
            return nullptr;
        }

        // set the subject and target of the new pair
        contact_pair->initialize_pair(this, temp_line);

        contact_pair->interaction_distance = (max_dimension + temp_line->max_dimension) * contact->contact_limit_factor;

        return contact_pair;
    }
}

/**
 * Find the point on the surface that coorisponds to the closest point on
 * line segments using the radius value
 * \param[in] nvec vector between line points
 */
void LineContactFacet::calculate_contact_point(double nvec[3])
{
    double end_point[3];
    double sign = 1.0;
    double unit[3];
    double vec[3];
    double x;

    if(!Numerical::compare_exact(contact_point[0], fabs(contact_point[0])))
    {
        sign = -sign;
    }
    Vector3::initialize(end_point);
    end_point[0] = sign * length / 2.0;

    // set x value to 0
    Vector3::normalize(nvec, vec);
    x = vec[0];
    vec[0] = 0.0;
    Vector3::normalize(vec, unit);
    if(Vector3::vmag(unit) > 1e-10)
    {
        // extend y z vector to surface of cylinder
        Vector3::scale(unit, radius, contact_point);
        // calculate new x based on ratio to y z vector\n
        contact_point[0] = contact_point[0] +
                           sign * sqrt(Numerical::square(x) *
                                       (Numerical::square(contact_point[1]) + Numerical::square(contact_point[2])) /
                                       (Numerical::square(vec[1]) + Numerical::square(vec[2])));
    }

    if(Numerical::fabs(contact_point[0]) >= Numerical::fabs(end_point[0]))
    {
        Vector3::diff(nvec, end_point, vec);
        Vector3::normalize(vec, unit);
        Vector3::scale(unit, radius, vec);
        Vector3::sum(end_point, vec, contact_point);
    }
}

/**
 * calculate the max dimension of the facet for range limit determination.
 */
void LineContactFacet::set_max_dimension()
{
    max_dimension = radius + length;
}

/**
 * Calculate the torque generated on the vehicle by the facet.  Assumes
 * that the force is in the vehicle structural frame, but that close_point is not.
 * \param[in] tmp_force force from one contact interaction.\n Units: N
 */
void LineContactFacet::calculate_torque(double * tmp_force)
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
