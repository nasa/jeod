/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time.cc
 * JeodBaseTime is an abstract class, containing the basic structure of all clocks that
 * run in JEOD.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time.o)
   (time__add_type_update.o)
   (time_manager.o)
   (time_manager_init.o)
   (time_messages.o)
   (utils/sim_interface/memory_interface.o)
   (utils/message/message_handler.o))

 

******************************************************************************/

/* System Includes */
#include <cstddef>

/* JEOD includes */
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"

/* Model Includes */
#include "../include/time.hh"
#include "../include/time_converter.hh"
//#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a JeodBaseTime
 */
JeodBaseTime::JeodBaseTime (
   void)
:    name(),
     links(*this)
{
   index       = 0;
   initialized = false;

   days               = 0;
   initial_value      = 0.0;
   clock_resolution   = 0.0001;
   initializing_value = 0.0;

   update_converter_direction = 0;
   seconds                    = 0.0;
   initialize_from_name       = "";
   update_from_name           = "";
   time_manager               = NULL;
   update_converter_ptr       = NULL;

   return;
}


/**
 * Default attempt to add a time-type to the initialization tree
 *
 * \par Assumptions and Limitations
 *  - Fails for TimeDyn, and has to be overwritten for others.
 * \param[in] seeking_status status-value for auto-seek
 * \param[in] time_manager_init TM initializer
 */
void
JeodBaseTime::add_type_initialize (
   const int seeking_status JEOD_UNUSED,
   TimeManagerInit * time_manager_init JEOD_UNUSED)
{
   MessageHandler::fail (
      __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
      "attempted to add a time-type to the initialization tree without the\n"
      "appropriate method.  Only types that inherit from TimeStandard or\n"
      "TimeUDE can be added, unless additional functionality is provided to\n"
      "override the default function inherited from JeodBaseTime.");
   return;
}

/**
 * Link the argument time as the update source for this time.
 *
 * \par Assumptions and Limitations
 *  - The linkage tree is currently implemented as a runtime inspection tool,
 *    and does not augment time update functionality.
 * \param[in] parent the time responsible for updating this time.
 */
void JeodBaseTime::add_parent( JeodBaseTime& parent )
{
    this->links.attach(parent.links);
}

/**
 * Default attempt to initialize a time-type from its parent
 *
 * \par Assumptions and Limitations
 *  - Fails for TimeDyn, and has to be overwritten for others.
 * \param[in] time_manager_init TM initializer
 */
void
JeodBaseTime::initialize_from_parent (
   TimeManagerInit * time_manager_init JEOD_UNUSED)
{
   MessageHandler::fail (
      __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
      "attempted to initialzie a time-type from its defined parent without the\n"
      "appropriate method.  Only types that inherit from TimeStandard or\n"
      "TimeUDE can be so initialized, unless additional functionality is\n"
      "iprovided to override the default function inherited from JeodBaseTime.");
   return;
}

/**
 * Virtual function that indicates if class must be a singleton
 * Defaults to yes
 * @return Boolean value
 */
bool
JeodBaseTime::must_be_singleton (
   void)
{
   return true;
}


/**
 * Updates each of the derived times from its parent time.
 *
 * \par Assumptions and Limitations
 *  - All but TimeDyn must have a parent; this should be defined by the
 *             user, or if not, already determined when the update_tree was built
 */
void
JeodBaseTime::update (
   void)
{
   if (update_converter_direction == 1) {
      update_converter_ptr->convert_a_to_b();
   }
   else if (update_converter_direction == -1) {
      update_converter_ptr->convert_b_to_a();
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::memory_error,
         "update_converter_direction has been set incorrectly or corrupted"
         " in time-class 'Time%s'\n",
         name.c_str());
   }

   return;
}

/**
 * Given a value of seconds, propagate to days.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_seconds new value for seconds\n Units: s
 */
void
JeodBaseTime::set_time_by_seconds (
   const double new_seconds)
{
   seconds = new_seconds;
   days    = seconds / 86400.0;
}


/**
 * Given a value of days, propagate to seconds.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_days new value for days\n Units: day
 */
void
JeodBaseTime::set_time_by_days (
   const double new_days)
{
   days    = new_days;
   seconds = days * 86400.0;
}


/**
 * Destroy a JeodBaseTime
 */
JeodBaseTime::~JeodBaseTime (
   void)
{
    // Destroy child link chain (cannot assume sublinks valid)
    while( links.has_children() )
    {
        TimeLinks* last_child = links.child_tail();
        last_child->detach();
    }
    links.detach(); // detach self from parent
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
