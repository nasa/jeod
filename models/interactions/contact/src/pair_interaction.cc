/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/pair_interaction.cc
 * A class to define the interaction type for a pair of contact facets.
 * This is a base class and derived classes define the force generation
 * function when contact between facets occurs.
 */

/************************** TRICK HEADER***************************************
PURPOSE:
 ()

REFERENCE:
    (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((pair_interaction.cc))


*******************************************************************************/

/* System includes */
#include <cstring>

/* Model includes */
#include "../include/contact_facet.hh"
#include "../include/contact_params.hh"
#include "../include/pair_interaction.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Check a pair of contact params for a match to the ones defined for
 * this pair_interaction.
 * @return bool
 * \param[in] subject_params parameters of the subject
 * \param[in] target_params parameters of the target
 */
bool PairInteraction::is_correct_interaction(ContactParams * subject_params, ContactParams * target_params)
{
    if(((subject_params->name == params_1) && (target_params->name == params_2)) ||
       ((target_params->name == params_1) && (subject_params->name == params_2)))
    {
        return true;
    }
    return false;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
