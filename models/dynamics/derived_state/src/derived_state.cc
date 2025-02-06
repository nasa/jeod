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
  ((derived_state.o)
   (derived_state_messages.o)
   (dynamics/mass/mass_point_state.o)
   (utils/sim_interface/memory_interface.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o)
   (utils/named_item/named_item_demangle.o)
   (utils/ref_frames/ref_frame.o)
   (utils/ref_frames/ref_frame_compute_relative_state.o))



*******************************************************************************/


// System includes
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
namespace jeod {

/**
 * Construct a DerivedState.
 */
DerivedState::DerivedState (
   void)
:
   subject(NULL),
   reference_name(NULL),
   state_identifier(NULL)
{
   return;
}


/**
 * Destruct a DerivedState.
 */
DerivedState::~DerivedState (
   void)
{
   if (JEOD_IS_ALLOCATED (reference_name)) {
      JEOD_DELETE_ARRAY (reference_name);
   }

   if (JEOD_IS_ALLOCATED (state_identifier)) {
      JEOD_DELETE_ARRAY (state_identifier);
   }

   return;
}


/**
 * Set the reference_name to a copy of the supplied value.
 * \param[in] new_name new name of reference.
 */
void
DerivedState::set_reference_name (
   const char * new_name)
{
   if (JEOD_IS_ALLOCATED (reference_name)) {
      JEOD_DELETE_ARRAY (reference_name);
      reference_name = NULL;
   }

   if (new_name != NULL) {
      reference_name = JEOD_STRDUP (new_name);
   }
   else {
      reference_name = NULL;
   }

   return;
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
void
DerivedState::initialize (
   DynBody & subject_body,
   DynManager & dyn_manager JEOD_UNUSED)
{

   // Save the input subject body as the subject for this derived state.
   subject = &subject_body;

   // Replicate the reference name so it can safely be freed.
   if ((reference_name != NULL) && (! JEOD_IS_ALLOCATED (reference_name))) {
      reference_name = JEOD_STRDUP (reference_name);
   }

   // Construct the derived state identifier.
   state_identifier =
      NamedItem::construct_name (
         NamedItem::demangle(typeid(*this)).c_str(),
         subject->name.c_str(), reference_name);

   return;
}


/**
 * Update the state.
 */
void
DerivedState::update (
   void)
{

   // Nothing to do here. This method exists because
   //   "A foolish consistency is the hobgoblin of small minds" (RWE).
   // The author is intentionally small-minded in this case.
   // All subclasses *must* pass the apply method to their parent class,
   // and therefore this method must exist so immediate subclasses can
   // satisfy this requirement.

   return;
}


/**
 * Find the Planet with the given name, failing if not found.
 * @return Found Planet
 * \param[in] dyn_manager Dynamics manager
 * \param[in] planet_name Planet name
 * \param[in] variable_name For error reporting
 */
Planet *
DerivedState::find_planet (
   DynManager & dyn_manager,
   const char * planet_name,
   const char * variable_name)
{
   Planet * found_planet = NULL;

   // Sanity check: The planet name must be provided.
   NamedItem::validate_name (__FILE__, __LINE__,
                             planet_name, "Planet", variable_name);

   // Find the Planet with the given name.
   found_planet = dyn_manager.find_planet (planet_name);

   // Sanity check: The planet_name must name a Planet.
   if (found_planet == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::invalid_name,
         "%s failed:\n"
         "Could not find planet named '%s' (variable %s)",
         state_identifier, planet_name, variable_name);

      // Not reached
      return NULL;
   }

   return found_planet;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
