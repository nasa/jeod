/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines DynBodyPendulumConstraint member functions.
 */

/*
Purpose: ()
*/


#include "../include/dyn_body_pendulum_constraint.hh"


//! Namespace jeod 
namespace jeod {

void
DynBodyPendulumConstraint::activate ()
{
    if (inactive)
    {
        DynBodyConstraint::activate ();
        pendulum_component.activate ();
    }
}


// Mark the constraint as inactive.
void
DynBodyPendulumConstraint::deactivate ()
{
    if (!inactive)
    {
        DynBodyConstraint::deactivate ();
        pendulum_component.deactivate ();
    }
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
