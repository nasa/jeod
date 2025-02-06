/*****************************************************************************

 Purpose:
 (GroundInteraction class for use with contact interaction model)

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((ground_interaction.cc))



 *****************************************************************************/

/* System includes */
#include <cmath>

/* JEOD includes */
#include "dynamics/dyn_body/include/body_ref_frame.hh"
#include "utils/math/include/vector3.hh"

/* Model includes */
#include "../include/ground_interaction.hh"

//! Namespace jeod
namespace jeod
{

/*******************************************************************************
 Function: initialize
 Purpose: (initialize terrain pointer and planet fixed coordinates)
 *******************************************************************************/
void GroundInteraction::initialize(Terrain * terrain_in) // Return: -- void
{
    terrain = terrain_in;
    point.initialize(terrain->planet);
    in_contact();
}

/*******************************************************************************
 Function: in_range
 Purpose: (test whether the pair is in range for interaction)
 *******************************************************************************/
bool GroundInteraction::in_range()
{
    double mag;

    mag = fabs(Vector3::vmag(point.cart_coords) - Vector3::vmag(subject->vehicle_point->state.trans.position));

    if(std::fpclassify(interaction_distance) == FP_ZERO || interaction_distance >= mag)
    {
        return true;
    }
    return false;
}

/*******************************************************************************
 Function: get_subject
 Purpose: (return a point to the subject of the ground interaction)
 *******************************************************************************/
ContactFacet * GroundInteraction::get_subject()
{
    return subject;
}

/*******************************************************************************
 Function: is_active
 Purpose: (deturmine if the subject and ground facets are active)
 *******************************************************************************/
bool GroundInteraction::is_active()
{
    if(subject->active && ground->active)
    {
        return true;
    }
    return false;
}
} // namespace jeod
