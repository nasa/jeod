/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/derived_state.cc
 * Define methods for the base body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((derived_state.cc)
   (derived_state_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc)
   (utils/named_item/src/named_item_demangle.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



*******************************************************************************/

// System includes
#include <array>
#include <cstddef>
#include <typeinfo>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/derived_state.hh"
#include "../include/derived_state_messages.hh"

//! Namespace jeod
namespace jeod
{
/**
 * Set the reference_name to a copy of the supplied value.
 * \param[in] new_name new name of reference.
 */
void DerivedState::set_reference_name(const std::string & new_name)
{
    reference_name = new_name;
}

/**
 * Begin initialization of a DerivedState.
 * The initialize method for all subclasses of DerivedState *nust* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 *
 * \par Assumptions and Limitations
 *  - g++ is being used as the compiler
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void DerivedState::initialize(DynBody & subject_body, DynManager & dyn_manager JEOD_UNUSED)
{
    // Save the input subject body as the subject for this derived state.
    subject = &subject_body;

    // Construct the derived state identifier.
    state_identifier = NamedItem::construct_name(NamedItem::demangle(typeid(*this)), subject->name.get_name());
    if(!reference_name.empty())
    {
        state_identifier = NamedItem::construct_name(state_identifier, reference_name);
    }
}

/**
 * Update the state.
 */
void DerivedState::update()
{
    // Nothing to do here. This method exists because
    //   "A foolish consistency is the hobgoblin of small minds" (RWE).
    // The author is intentionally small-minded in this case.
    // All subclasses *must* pass the apply method to their parent class,
    // and therefore this method must exist so immediate subclasses can
    // satisfy this requirement.
}

/**
 * Find the Planet with the given name, failing if not found.
 * @return Found Planet
 * \param[in] dyn_manager Dynamics manager
 * \param[in] planet_name Planet name
 * \param[in] variable_name For error reporting
 */
Planet * DerivedState::find_planet(const DynManager & dyn_manager,
                                   const std::string & planet_name,
                                   const std::string & variable_name)
{
    Planet * found_planet = nullptr;

    // Sanity check: The planet name must be provided.
    NamedItem::validate_name(__FILE__, __LINE__, planet_name, "Planet", variable_name);

    // Find the Planet with the given name.
    found_planet = dyn_manager.find_planet(planet_name);

    // Sanity check: The planet_name must name a Planet.
    if(found_planet == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             DerivedStateMessages::invalid_name,
                             "%s failed:\n"
                             "Could not find planet named '%s' (variable %s)",
                             state_identifier.c_str(),
                             planet_name.c_str(),
                             variable_name.c_str());

        // Not reached
        return nullptr;
    }

    return found_planet;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
