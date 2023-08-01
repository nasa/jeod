/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time__add_type_update.cc
 * Define JeodBaseTime::add_type_update.
 * This is a final method that draws in a lot of the time model functionality.
 * Making this method a separate compilation unit enables models that only need
 * the vtable for class Time can get that from time.o without pulling in the
 * whole of the time model.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time.cc)
   (time_manager.cc)
   (time_manager_init.cc)
   (time_messages.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc))

 

******************************************************************************/

/* System Includes */
#include <cstddef>

/* JEOD includes */
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"

/* Model Includes */
#include "../include/time.hh"
#include "../include/time_converter.hh"
#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Recursively adds elements to the update tree.
 * If the "parent" to a time-type is defined, adds the "parent" then
 * returns to adding the "child" type.
 * If the "parent" is not defined it searches for a suitable "parent"
 * from the types already in the tree.  If that search is successful,
 * it adds the "child" to the tree, otherwise it returns without
 * change.
 *
 * \par Assumptions and Limitations
 *  - None
 * \param[in] seeking_status status-value for auto-seek.
 * \param[in] time_manager_init The TM initializer.
 */
void
JeodBaseTime::add_type_update (
   const int seeking_status,
   TimeManagerInit * time_manager_init)
{
   int conv_index = 0; // -- converter index in TimeManagerInit
   int conv_dir = 0;       // -- direction of application of converter
   JeodBaseTime * parent_ptr = nullptr;  // -- pointer to the parent time.
   int parent_index = -1;   // -- index-value of the parent time

   time_manager_init->set_status (index, -1);
   // used to check for loops; resets before exit

   parent_index = time_manager->time_lookup (update_from_name);

   // If parent name not found in registered times: failure
   if (parent_index == -1) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "An attempt was made to define the source"
         " of the update of %s as being '%s', but '%s' was not"
         " recognized by the time manager as a valid name.\n"
         " Error likely in Time %s.update_from_name \n",
         name.c_str(),
         update_from_name.c_str(),
         update_from_name.c_str(),
         name.c_str());
   }
   // else if parent name is blank, do an auto-seek.
   else if (parent_index == -2) {
      for (int ii = 0; ii < time_manager->num_types; ++ii) {
         if (time_manager_init->get_status (ii) == seeking_status) {
            // TimeManagerInit converter unique index formula:
            conv_index = ii * time_manager->num_types + index;
            conv_dir   = time_manager_init->get_conv_dir_upd (conv_index);
            if (conv_dir != 0) { // found a parent.
               parent_index = ii;
               parent_ptr   = time_manager->get_time_ptr (ii);
               add_parent(*parent_ptr);
               MessageHandler::inform (
                  __FILE__, __LINE__, TimeMessages::incomplete_setup_error,
                  "\n"
                  "The update tree was not fully defined by the user: \n"
                  "Time-type %s has no defined update path.\n"
                  "It will be updated from time-type %s.\n",
                  name.c_str(),
                  parent_ptr->name.c_str());

               break;
            }
         }
      }
      // if through all registered times and have not found any parent:
      if (parent_index == -2) {
         // cannot be added to the tree at this time.
         // Remain uninitialized for now.
         time_manager_init->set_status (index, 0);
         return;
      }
   }
   else { // parent defined and found
      // Link it in
      int parent_status = time_manager_init->get_status (parent_index);
      // 1. verify that this is not a loop.
      if (parent_status == -1) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
            "In defining the tree for updating the time-type, an infinite loop\n"
            "was encountered with the pre-defined 'time.time_XXX.update_from'\n"
            "values. Time-type '%s' depends on time-type '%s', and vice-versa. \n",
            update_from_name.c_str(),
            name.c_str());
      }

      // 2. check that a converter exists to do this task
      conv_index = parent_index * time_manager->num_types + index;
      conv_dir   = time_manager_init->get_conv_dir_upd (conv_index);
      if (conv_dir == 0) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
            "An attempt was made to define time-type '%s'"
            "as the source for updating %s, but no converter was provided to"
            " convert from %s to %s.  \n",
            time_manager->get_time_ptr (parent_index)->name.c_str(),
            name.c_str(),
            time_manager->get_time_ptr (parent_index)->name.c_str(),
            name.c_str());
      }

      // 3. if the parent is not in the tree, add it
      //  Two possibilities here - build the update tree by shoots (recursive
      //  addition), or in layers.  Recursive addition is more efficient (so add
      //  the ..., then the grandparent, then the parent, then this).  This was
      //  the original intention, but it has the following drawback:
      //  Suppose we have a serial/linear tree with one object at each of level
      //  1,2,3,4, and one additional (poorly defined) type that should go in
      //  level 5.  Suppose we are now in level 3, the other types have already
      //  been added in the level-2 sweep, the autoseek looks for the parent to
      //  the remaining type at level 2, finds none, and cannot add anything,
      //  so the code fails out. The only prevention of this is to maintain a
      //  value for the maximum depth of the tree so that the code only fails
      //  when the autoseek has tested the deepest levels.  But this eliminates
      //  the efficiency of building the tree recursively.
      //  The alternative, building by layers, would attempt to add the
      //  parent to the current level, and then not attempt to add this type,
      //  because this type will be added at the next level.
      parent_ptr = time_manager->get_time_ptr (parent_index);
      // If first loop, construct links
      if( links.parent() == nullptr )
      {
          add_parent(*parent_ptr);
      }
      // if subsequent recursion, check consistent
      else if( links.parent() != parent_ptr )
      {
          MessageHandler::fail (
                  __FILE__, __LINE__, TimeMessages::invalid_node, "\n"
                  "Time converter attempting to double parent."
                  "Time (%s) has parent time (%s), but attempting to \n"
                  "re-parent to time (%s). "
                  "Unable to reconcile time update tree.\n",
                  name.c_str(), links.parent()->name.c_str(),
                  parent_ptr->name.c_str() );
      }

      if (parent_status == 0) {
         parent_ptr->add_type_update (seeking_status, time_manager_init);
         //parent_status = time_manager_init->get_status (parent_index);
           // DO NOT RESET if building by layers.
         //      }
      // 4. if parent could not be added, reset status of this type to 0
      //    (indicating failed placement) and exit.
         //      if (parent_status == 0) {
           //  Do not need to test anymore, it must be = 0 since it hasn't changed
           //  in previous if-block.  Also, if adding parent, do not want to try
           //  adding this type now when working on a layered approach, it
           //  must bounce out.
         time_manager_init->set_status (index, 0);
         return;
      }
   }

   //  Get here when parent either defined or found, and it has already been
   //  placed in the tree ( status[parent_index] > 0).

   int mgr_conv_ix = time_manager_init->get_conv_ptr_index (
      conv_index);
   TimeConverter * converter_ptr = time_manager->get_converter_ptr (mgr_conv_ix);
   if (converter_ptr == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::memory_error, "\n"
         "No converter found at index %i in the time_manager's converter registry"
         ", mapped from \n"
         "time_manager_init, derived from the time types %s and %s,\n"
         "with indices %i and %i respectively.\n",
         conv_index, name.c_str(), parent_ptr->name.c_str(), index, parent_index);
      return;
   }

   if (!converter_ptr->is_initialized()) {
      converter_ptr->initialize (
         parent_ptr,
         this,
         conv_dir);
   }
   update_converter_ptr       = converter_ptr;
   update_converter_direction = conv_dir;

   time_manager_init->increment_status (index, parent_index);
   time_manager_init->num_added_total++;

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
