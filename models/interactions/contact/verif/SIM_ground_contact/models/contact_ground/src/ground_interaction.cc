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
#include "utils/math/include/vector3.hh"
#include "dynamics/dyn_body/include/body_ref_frame.hh"

/* Model includes */
#include "../include/ground_interaction.hh"

//! Namespace jeod
namespace jeod {

/*******************************************************************************
 Function: GroundInteraction
 Purpose: (Default Constructor)
 *******************************************************************************/
GroundInteraction::GroundInteraction (
                          void)
: // Return: -- None
subject (nullptr),
ground (nullptr),
interaction (nullptr),
interaction_distance (0.0),
friction_mag (0.0),
velocity_mag (0.0),
terrain (nullptr)
{
   Vector3::initialize(ground_normal);
}


/*******************************************************************************
 Function: ~GroundInteraction
 Purpose: (Destructor)
 *******************************************************************************/
GroundInteraction::~GroundInteraction ( // Return: -- void
                           void)
{

}

/*******************************************************************************
 Function: initialize
 Purpose: (initialize terrain pointer and planet fixed coordinates)
 *******************************************************************************/
void
GroundInteraction::initialize (
                          Terrain * terrain_in) // Return: -- void
{
   terrain = terrain_in;
   point.initialize(terrain->planet);
   in_contact();
}

/*******************************************************************************
 Function: in_range
 Purpose: (test whether the pair is in range for interaction)
 *******************************************************************************/
bool
GroundInteraction::in_range ( // Return: -- bool
   void)
{
   double mag;

   mag = fabs(Vector3::vmag(point.cart_coords) -
      Vector3::vmag(subject->vehicle_point->state.trans.position));

   if (std::fpclassify(interaction_distance) == FP_ZERO || interaction_distance >= mag) {
      return true;
   }
   return false;
}

/*******************************************************************************
 Function: get_subject
 Purpose: (return a point to the subject of the ground interaction)
 *******************************************************************************/
ContactFacet *
GroundInteraction::get_subject ( // Return: -- ContactFacet pointer
   void)
{
   return subject;
}

/*******************************************************************************
 Function: is_active
 Purpose: (deturmine if the subject and ground facets are active)
 *******************************************************************************/
bool GroundInteraction::is_active ( // Return: -- bool
   void)
{
   if(subject->active && ground->active) {
      return true;
   }
   return false;
}
} // End JEOD namespace
