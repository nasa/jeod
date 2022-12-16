/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemItem
 * @{
 *
 * @file models/environment/ephemerides/ephem_item/src/ephem_item.cc
 * Define member functions for the EphemItem class and subclasses.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((ephem_item.o)
   (environment/ephemerides/ephem_manager/ephem_manager.o)
   (utils/memory/memory_manager_static.o)
   (utils/memory/memory_type.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o)
   (utils/ref_frames/subscription.o))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/ephemerides/ephem_manager/include/base_ephem_manager.hh"
#include "environment/planet/include/base_planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/ephem_item.hh"



//! Namespace jeod
namespace jeod {

/**
 * Construct an ephemeris item.
 */
EphemerisItem::EphemerisItem (
   void)
:
   // subscribers(0),
   name(NULL),
   owner(NULL),
   manager(NULL),
   target_frame(NULL),
   head(NULL),
   next(NULL),
   update_time(0.0),
   enabled(false),
   active(false)
{
   ;  // Empty; all items initialized in initializer list
}


/**
 * Destroy an ephemeris item.
 */
EphemerisItem::~EphemerisItem (
   void)
{
   if ((name != NULL) && (JEOD_IS_ALLOCATED (name))) {
      JEOD_DELETE_ARRAY (name);
      name = NULL;
   }
}


/**
 * Name an ephemeris item.
 * \param[in] file Usually __FILE__
 * \param[in] line Usually __LINE__
 * \param[in] new_value Value to check
 * \param[in] old_value Current value
 * \param[in] variable_name Variable name
 */
void
EphemerisItem::validate_name (
   const char * file,
   unsigned int line,
   const char * new_value,
   const char * old_value,
   const char * variable_name)
{

   // The name must not be trivially valid (not null or empty).
   NamedItem::validate_name (
      file, line, new_value, "Argument", variable_name);

   // The value cannot be changed once registered with the ephemerides manager.
   if ((manager != NULL) &&
       (old_value != NULL)  &&
       (std::strcmp (old_value, new_value) != 0)) {
      MessageHandler::fail (
         file, line, EphemeridesMessages::invalid_name,
         "Illegal attempt to change ephemeris item %s from '%s' to '%s'.",
         variable_name, old_value, new_value);
   }
}


/**
 * Name an ephemeris item.
 * \param[in] pname Planet name
 * \param[in] fname Frame name
 */
void
EphemerisItem::set_name (
   const char * pname,
   const char * fname)
{

   // Validate the name parts.
   validate_name (__FILE__, __LINE__, pname, name, "pname");
   validate_name (__FILE__, __LINE__, fname, name, "fname");

   // Construct and set the item name.
   set_name_internal (NamedItem::construct_name (pname, fname));
}


/**
 * Name an ephemeris item.
 * \param[in] new_name New name
 */
void
EphemerisItem::set_name (
   const char * new_name)
{

   // Validate the name.
   validate_name (__FILE__, __LINE__, new_name, name, "new_name");

   // Ephemeris reference frames have a name of the form "xxx.yyy".
   // Lacking such a form, assume the caller meant the ".yyy" to be the
   // default suffix for this item class.
   if (std::strchr (new_name, '.') == NULL) {
      MessageHandler::inform (
         __FILE__, __LINE__, EphemeridesMessages::invalid_name,
         "Ephemeris item names must be of the form \"object.frame_name\".\n"
         "Input value \"%s\" is invalid. Using \"%s.%s\".",
         new_name, new_name, default_suffix());
      set_name (new_name, default_suffix());
   }

   // Name is of the form "xxx.yyy":
   // Set the name per the argument, but do so via a string copy.
   else {
      set_name_internal (JEOD_STRDUP (new_name));
   }
}


/**
 * Name an ephemeris item.
 * \param[in] new_name New name
 */
void
EphemerisItem::set_name_internal (
   char * new_name)
{

   // Free an existing value if it was allocate by JEOD.
   if ((name != NULL) && (JEOD_IS_ALLOCATED (name))) {
      JEOD_DELETE_ARRAY (name);
   }

   // Set the name to the given value.
   name = new_name;
}


/**
 * Set the target frame.
 * All ephemeris items that share a common name must point
 * to the same target frame.
 * \param[in] frame Target frame
 */
void
EphemerisItem::set_target_frame (
   EphemerisRefFrame & frame)
{
   // The head item needs to handle this message.
   if (this != head) {
      head->set_target_frame (frame);
      return;
   }

   // Nothing to do if the target frame has already been set on head and all
   // connected EphemItems.  If it has been set on the head item, set it on any
   // follow-on items that have not already been set.
   if (target_frame == &frame) {
      for (EphemerisItem * ephem_item = head;
           ephem_item != NULL;
           ephem_item = ephem_item->next) {
         if (ephem_item->target_frame == NULL) {
            ephem_item->target_frame = &frame;
         }
      }
      return;
   }

   // Items that update translational state must have integration frames as
   // their target, while items that update rotational state should target
   // non-integration frames.
   if ((updates_what() == EphemerisItem::Translation) !=
       manager->is_integ_frame (frame)) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::invalid_item,
         "Incompatible item type and target frame for ephemeris item '%s'.",
         name);
      return;
   }

   // Changing the target frame is verboten.
   if (target_frame != NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::invalid_item,
         "Illegal attempt to change the target frame for ephemeris item '%s'.",
         name);
      return;
   }

   // Set the target frame for all ephemeris items in the list.
   for (EphemerisItem * ephem_item = head;
        ephem_item != NULL;
        ephem_item = ephem_item->next) {
      ephem_item->target_frame = &frame;
   }

   // Make the enabled item, if there is one, be the 'owner' of this frame.
   EphemerisItem * enabled_item = get_enabled_item ();
   if (enabled_item != NULL) {
      target_frame->set_owner (enabled_item);
      if (target_frame->is_active()) {
         enabled_item->activate();
      }
   }
}


/**
 * Enable an EphemerisItem object.
 */
void
EphemerisItem::enable (
   void)
{
   // Nothing to do if the object is already enabled.
   if (enabled) {
      return;
   }

   EphemerisItem * enabled_item = get_enabled_item ();

   // If there is a currently enabled item,
   // Set the activity of this object to that of currently-enabled item and
   // disable the enabled object.
   if (enabled_item != NULL) {
      active = enabled_item->active;
      enabled_item->disable ();
   }

   // Transfer ownership of the target frame to this object.
   if (target_frame != NULL) {
      target_frame->set_owner (this);
   }

   // Let the manager know that things have changed.
   if (manager != NULL) {
      manager->ephem_note_tree_status_change ();
   }

   // Finally, mark the object as enabled.
   enabled = true;
}


/**
 * Disable an EphemerisItem object.
 */
inline void
EphemerisItem::disable (
   void)
{
   // Nothing to do if the object is not enabled.
   if (! enabled) {
      return;
   }

   // Transfer ownership of the target frame away from this object.
   if (target_frame != NULL) {
      target_frame->set_owner (NULL);
   }

   // Let the manager know that things have changed.
   if (manager != NULL) {
      manager->ephem_note_tree_status_change ();
   }

   // Finally, mark the object as disabled and inactive.
   enabled = false;
   active  = false;
}



/**
 * Activate a EphemerisItem object.
 */
void
EphemerisItem::activate (
   void)
{
   if (is_activatable()) {
      active = true;
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, EphemeridesMessages::invalid_item,
         "Ephemeris item '%s' is not activatable.",
         name);
   }
}


/**
 * Is the item activatable?
 * @return True if item can be activated.
 */
bool
EphemerisItem::is_activatable (
   void)
const
{
   // An item must be enabled before it can be activated.
   if (! enabled) {
      return false;
   }

   // An item that is already activated is (trivially) activatable.
   if (active) {
      return true;
   }

   // Only one item with the same name can be active.
   for (EphemerisItem * ephem_item = head;
        ephem_item != NULL;
        ephem_item = ephem_item->next) {
      if (ephem_item->active) {
         return false;
      }
   }

   return true;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
