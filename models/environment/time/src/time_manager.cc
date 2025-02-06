/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_manager.cc
 * Define member functions for class TimeManager.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_manager.o)
   (time_manager__initialize.o)
   (time.o)
   (time_dyn.o)
   (time_manager_init.o)
   (time_messages.o)
   (time_standard.o)
   (utils/integration/jeod_integration_time.o)
   (utils/sim_interface/memory_interface.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o))

 
******************************************************************************/

// System includes
#include <algorithm>
#include <cstddef>
#include <cstring>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/time.hh"
#include "../include/time_converter.hh"
#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"
#include "../include/time_standard.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeManager
 */
TimeManager::TimeManager (
   void)
:
   simtime(-1.0), // Forces update at initialization
   num_types(0),
   time_change_flag(false)
{

   // Register types and objects for checkpoint/restart.
   JEOD_REGISTER_CLASS(TimeManager);
   JEOD_REGISTER_CLASS(JeodBaseTime);
   JEOD_REGISTER_CLASS(JeodBaseTime*);
   JEOD_REGISTER_CLASS(TimeConverter);
}


/**
 * Return a pointer to the TimeConverter object with the provided index,
 * or NULL if no such TimeConverter object has been registered.
 * @return TimeConverter object corresponding to index in the vector of such types.
 * \param[in] index Index of object
 */
TimeConverter *
TimeManager::get_converter_ptr (
   const int index)
const
{
   TimeConverter * converter_ptr;

   if (index < 0) {
      converter_ptr = NULL;
   }
   else {
      converter_ptr = converter_vector[index];
   }
   return converter_ptr;
}


/**
 * Expose the private inheritance from JeodIntegrationTime.
 */
JeodIntegrationTime&
TimeManager::get_jeod_integration_time (
   void)
{
   return *(dynamic_cast<JeodIntegrationTime*> (this));
}


/**
 * Returns the boolean value time_change_flag
 * @return time_change_flag
 */
bool
TimeManager::get_time_change_flag (
   void)
const
{
   return time_change_flag;
}


/**
 * Returns the scale factor from sim time to dynamic time.
 * @return dyn_time.scale_factor
 */
double
TimeManager::get_time_scale_factor (
   void)
const
{
   return dyn_time.scale_factor;
}


/**
 * Returns the time used to timestamp objects,
 * currently dynamic time seconds.
 * @return dyn_time.seconds
 */
double
TimeManager::get_timestamp_time (
   void)
const
{
   return dyn_time.seconds;
}


/**
 * Return a pointer to the Time object with the provided name,
 * or NULL if no such Time object has been registered.
 * @return Time object corresponding to name
 * \param[in] name Name of time object
 */
JeodBaseTime *
TimeManager::get_time_ptr ( const std::string& name) const
{
   const int index = time_lookup (name);

   return get_time_ptr (index);
}


/**
 * Return a pointer to the Time object with the provided index,
 * or NULL if no such Time object has been registered.
 * @return Time object corresponding to name
 * \param[in] index Name of time object
 */
JeodBaseTime *
TimeManager::get_time_ptr (
   const int index)
const
{
   JeodBaseTime * time_ptr;
   if (index < 0) {
      time_ptr = NULL;
   }
   else {
      time_ptr = time_vector[index];
   }
   return time_ptr;
}


/**
 * Registers the time representation with the Time Manager.  Records the
 * frequency at which the representation should be updated.
 * \par Assumptions and Limitations
 *  - None
 * \param[in,out] time_ref reference to time-type being registered
 */
void
TimeManager::register_time (
        JeodBaseTime & time_ref)
{
    if( std::find(time_vector.begin(), time_vector.end(), &time_ref) != time_vector.end() )
    {
        MessageHandler::inform(
                __FILE__,__LINE__,TimeMessages::redundancy_error,"\n"
                "Ignoring attempt to re-register time (%s), \n"
                "because it is already registered with the TimeManager.\n",
                time_ref.name.c_str() );
        return;
    }
    time_ref.time_manager = this;

    time_vector.push_back (& time_ref);

    /* keep a record of where each time type appears in the manager, this saves
      on looking up by name. */
    time_ref.set_index( time_vector.size() - 1 );

    return;
}


/**
 * Reassigns the name to the type; this is used when there are multiple instances of a time type such as a MET or UDE.  Registers the time representation with the Time Manager.  Records the
 * frequency at which the representation should be updated.
 * TODO: check for duplicates
 * \par Assumptions and Limitations
 *  - None
 * \param[in,out] time_ref reference to time-type being registered
 * \param[in] name name of the instance being registered.
 */
void
TimeManager::register_time_named (
   JeodBaseTime & time_ref,
   const std::string& name)
{
   if ( name != time_ref.name.get_name() ) {
      MessageHandler::warn(
         __FILE__,__LINE__,"Registration override:\n",
         "The original name of this time-type (%s) is being overwritten\n"
         "with a new name(%s).\n",
         time_ref.name.c_str(),
         name.c_str());
      time_ref.name = name;
   }

   register_time (time_ref);
   return;
}


/**
 * Registers the time converters with the Time Manager.
 *
 * \par Assumptions and Limitations
 *  - the input values name_a and name_b will only be used if the
 *                converter-type names have not already been set.  So registering a
 *                Dyn_UDE converter will ignore name_a completely because it is
 *                already set.
 * \param[in,out] conv_ref ref. to converter being registered
 * \param[in] name_a name of type-a in the converter
 * \param[in] name_b name of type-b in the converter
 */
void
TimeManager::register_converter (
   TimeConverter & conv_ref,
   std::string name_a,
   std::string name_b)
{
   converter_vector.push_back (& conv_ref);
   if( conv_ref.a_name.empty() ) {
       if( !name_a.empty() ) {
           conv_ref.a_name = name_a;
       }
       else {
           MessageHandler::fail (
                    __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
                    "Arbitrary specification while registering a converter.\n"
                    "User failed to specify the name of the first time-type,\n"
                    "which has no default name." );
       }
   }
   else if ( !name_a.empty() && conv_ref.a_name != name_a ) {
      MessageHandler::inform (
         __FILE__, __LINE__, TimeMessages::redundancy_error, "\n"
         "Redundant specification while registering a converter.\n"
         "User registered the name of the first time-type as '%s', \n"
         "but previously the first time-type\n"
         "was defined as that with name '%s'.\n"
         "The user-specified name is being ignored.\n",
         name_a.c_str(), conv_ref.a_name.c_str() );
   }
   if( conv_ref.b_name.empty() ) {
       if( !name_b.empty() ) {
           conv_ref.b_name = name_b;
       }
       else {
           MessageHandler::fail (
                    __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
                    "Arbitrary specification while registering a converter.\n"
                    "User failed to specify the name of the second time-type,\n"
                    "which has no default name." );
       }
   }
   else if ( !name_b.empty() && conv_ref.b_name != name_b ) {
      MessageHandler::inform (
         __FILE__, __LINE__, TimeMessages::redundancy_error, "\n"
         "Redundant specification while registering a converter.\n"
         "User registered the name of the second time-type as '%s', \n"
         "but previously the second time-type\n"
         "was defined as that with name '%s'.\n"
         "The user-specified name is being ignored.\n",
         name_b.c_str(), conv_ref.b_name.c_str() );
   }

   return;
}


/**
 * Tests for the existence in the registry of time types that inherit
 * from TimeStandard.
 *
 * \par Assumptions and Limitations
 *  - None
 * @return true/false
 */
bool
TimeManager::time_standards_exist (
   void)
{
   for (unsigned ii = 0; ii < time_vector.size(); ii++) {
      JeodBaseTime * time_ptr     = time_vector[ ii ];
      TimeStandard * time_ptr_std = dynamic_cast<TimeStandard *> (time_ptr);
      if (time_ptr_std != NULL) {
         return true;
      }
   }
   return false;
}


/**
 * Uses a string comparison to find where in the TimeManager record a
 * time type of a particular name is located.  Returns the integer
 * corresponding to the time type's index in the TimeManager.
 *
 * \par Assumptions and Limitations
 *  - Rarely used.  If the time type address is known, it is easier to
 *    access its index "time_type.index" which returns the same result.
 * @return index value of time-type
 * \param[in] name name of time-type
 */
int TimeManager::time_lookup ( const std::string& name ) const
{
   int index = -1; // -1 corresponds to unfound

   // Sanity check:
   // Test for a NULL or empty input name.
   if ( name.empty() )
   {
       index = -2; // -2 corresponds to search name  error
   }

   // Input name is of a valid form.
   // Find the matching item, if any.
   else {
      for (unsigned ii = 0; ii < time_vector.size(); ++ii) {
         // See if we have a match to time_type_ptrs[ii].
         // First, sanity check that time_types_ptrs[ii]->name is valid
         // If so, test whether the name of that item matches the input.
         if ( name == time_vector[ ii ]->name ) {

            // Have a match.
            // See if we already had a match.
            // This should never happen (it means there are 2
            // time types with the same name), but just in case ...
            if (index > -1) {
               MessageHandler::fail (
                  __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
                  "Looked up time type by name (%s) but found two"
                  " occurrences.  Unable to distinguish between them.\n",
                  name.c_str());
               return -3;
            }
            else {
               index = ii;
            }
         }
      }
   }

   //  returns -1 if no match found, -2 if found an "undefined".
   //  Check these at each calling function for appropriate action.

   return(index);
}


/**
 * This function manages the time update process.
 * It first updates each of the representations of time, calling the
 * update functions for each time representation in dependency order.
 * After updating the representations of time, the function then
 * updates the dynamic time scale factor. Time change subscribers are
 * notified if the scale factor has changed.
 *
 * Note that by updating first and then checking for a change in the
 * rate/direction of time means that these changes in rate/direction
 * will first take affect on the next call to update_time or update.
 *
 * \par Assumptions and Limitations
 *  - Derived times must have a parent; this should be defined by the
 *         user, or if not, already determined when the update_tree was built
 * \param[in] current_simtime input time from simulation engine; it always runs forwards and allows for determination of what has and has not already been done.\n Units: s
 */
void
TimeManager::update (
   double current_simtime)
{

   if (!Numerical::compare_exact(current_simtime,simtime)) {
      simtime = current_simtime;

      // update all times that are to be updated with the manager
      //   These are ordered in some update hierarchy, such that if x updates
      //   from y, y appears in the ordered_update_list array first.
      for (int ii = 0; ii < num_types; ++ii) {
         time_vector[ ii ]->update();
      }

   }

   // check for run-time changes to scaling factor or time direction.
   time_change_flag = dyn_time.update_offset();

   // notify subscribers if time has changed meaning.
   if (time_change_flag) {
      notify_time_change_subscribers();
   }

   return;
}


/**
 * Update each of the representations of time, calling the update
 * functions for each such representation in dependency order.
 *
 * Note that this function only does the first part of the task
 * performed by TimeManager::update. It does not check for changes
 * in the rate/direction of time.
 *
 * \par Assumptions and Limitations
 *  - Derived times must have a parent; this should be defined by the
 *         user, or if not, already determined when the update_tree was built
 * \param[in] current_simtime input time from simulation engine; it always runs forwards and allows for determination of what has and has not already been done.\n Units: s
 */
void
TimeManager::update_time (
   double current_simtime)
{

   if (!Numerical::compare_exact(current_simtime,simtime)) {
      simtime = current_simtime;

      // update all times that are to be updated with the manager
      //   These are ordered in some update hierarchy, such that if x updates
      //   from y, y appears in the ordered_update_list array first.
      for (int ii = 0; ii < num_types; ++ii) {
         time_vector[ ii ]->update();
      }

   }

   return;
}


/**
 * This function is called when the simulation reverses direction (in
 * time.  It calls each time converter that uses a table lookup to check
 * whether the current time is off the end of the table.  This is
 * important because once the off-table-end flag is set, the only reason
 * to unset it is when time reverses direction)
 *
 * \par Assumptions and Limitations
 *  - None
 */
void
TimeManager::verify_table_lookup_ends (
   void)
{
   for (unsigned int ii = 0; ii < converter_vector.size(); ++ii) {
      converter_vector[ ii ]->verify_table_lookup_ends();
   }
   return;
}


/**
 * Destroy a TimeManager
 */
TimeManager::~TimeManager (
   void)
{

   time_vector.clear();
   converter_vector.clear();

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
