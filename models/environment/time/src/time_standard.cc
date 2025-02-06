/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_standard.cc
 * An abstract class, this defines the basic structure of member functions for
 * all Standard Times.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_standard.o)
   (time.o)
   (time_manager.o)
   (time_manager_init.o)
   (time_messages.o)
   (utils/sim_interface/memory_interface.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o))

 
******************************************************************************/

// System includes
#include <cmath>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/time_standard.hh"
#include "../include/time_converter.hh"
#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeStandard
 */
TimeStandard::TimeStandard (
   void)
:
   JeodBaseTime(),
   last_calendar_update(-100000.0),  // ridiculous value, allows calendar
                                     //   update at initialization
   prev_julian_day(-1000000000),     // ridiculous value, forces full calendar
                                     //    calculation at first call.
   seconds_at_year_start(0.0),
   year_of_last_soy(-1000000),       // ridiculous value, forces seconds_of_year
                                     //into full calculation first time through.
   send_warning_pre_1968(true),
   tjt_mjt_offset(40000.0),
   tjt_jd_offset(2440000.5),
   trunc_julian_time(0.0),
   julian_date(0.0),
   tjt_at_epoch(0.0),
   calendar_day(0),
   calendar_hour(0),
   calendar_minute(0),
   calendar_second(0.0),
   calendar_year(0),
   calendar_month(0)
{}

/**
 * Given a value of seconds, propagate to days and trunc_julian_time.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_seconds new value for seconds\n Units: s
 */
void
TimeStandard::set_time_by_seconds (
   const double new_seconds)

{
   JeodBaseTime::set_time_by_seconds (new_seconds);
   trunc_julian_time = tjt_at_epoch + days;
   julian_date = trunc_julian_time + tjt_jd_offset;
   return;

}

/**
 * Given a value of days, propagate to seconds and trunc_julian_time.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_days new value for days\n Units: day
 */
void
TimeStandard::set_time_by_days (
   const double new_days)


{
   JeodBaseTime::set_time_by_days (new_days);
   trunc_julian_time = tjt_at_epoch + days;
   julian_date = trunc_julian_time + tjt_jd_offset;
   return;
}

/**
 * Given a value of tjt, propagate to seconds and days.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_tjt new value for Truncated Julian Time\n Units: day
 */
void
TimeStandard::set_time_by_trunc_julian (
   const double new_tjt)

{
   trunc_julian_time = new_tjt;
   days              = trunc_julian_time - tjt_at_epoch;
   seconds           = days * 86400.0;
   julian_date = trunc_julian_time + tjt_jd_offset;
   return;
}

/**
 * Returns the full Julian date at epoch, rather than the Truncated
 * Julian Time
 * @return Truncated Julian Time at the epoch of the time-type.\n Units: day
 */
double
TimeStandard::julian_date_at_epoch (
   void)
{

   return tjt_at_epoch + tjt_jd_offset;

}


/**
 * Recursively adds elements to the initialization tree.
 * If the "parent" to a time-type is defined, adds the "parent" then
 * returns to adding the "child" type.
 * If the "parent" is not defined it searches for a suitable "parent"
 * from the types already in the tree.  If that search is successful,
 * it adds the "child" to the tree, otherwise it returns without
 * change.
 *
 * \par Assumptions and Limitations
 *  - This is vastly improved if the user defines the parent type
 *      "initialize_from" for each time representation, except the
 *      top-level initializer type.
 *  - Otherwise, the code will build the tree automatically,
 *      but it takes longer and may be less than ideal
 * \param[in] seeking_status status-value for auto-seek
 * \param[in] time_manager_init The TM initializer.
 */
void
TimeStandard::add_type_initialize (
   const int seeking_status,
   TimeManagerInit * time_manager_init)
{
   int conv_index;       // -- converter index in TimeManagerInit
   int parent_index;     // -- index-value of the parent time-type
   int conv_dir;         // -- direction in which to apply the converter

   time_manager_init->set_status (index, -1);
   // used to check for loops; resets before exit

   // Look up the index value of the parent
   parent_index = time_manager->time_lookup (initialize_from_name);

   //  If parent undefined, do an auto-seek.
   if (parent_index == -2) {
      for (int jj = 0; jj < time_manager->num_types; ++jj) {
         if (time_manager_init->get_status (jj) == seeking_status) {
            conv_index = jj * time_manager->num_types + index; /* --
                    converter index in TimeManagerInit registry. */
            conv_dir   = time_manager_init->get_conv_dir_init (conv_index);
            if (conv_dir != 0) {
               initialize_from_name = time_manager->get_time_ptr (jj)->name.get_name();
               MessageHandler::inform (
                  __FILE__, __LINE__, TimeMessages::incomplete_setup_error,
                  "\n"
                  "The initialization tree was not fully defined by the user.\n"
                  "Time-type %s has no defined initialization path.\n"
                  "It will be initialized from time-type %s.\n",
                  name.c_str(),
                  initialize_from_name.c_str());
               time_manager_init->increment_status (index, jj);
               return;
            }
         }
      }
      // went through all types, didn't find anything suitable.
      // cannot be added to the tree at this time
      time_manager_init->set_status (index, 0);
      return;
   }
   // else
   //  If parent defined but not found, terminate
   if (parent_index == -1) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
         "An attempt was made to define %s as the source"
         " for the initialization of time type %s, but time type %s was not"
         " recognized by the time manager as a valid name.\n"
         "Error likely in Time%s.initialize_from_name. \n",
         initialize_from_name.c_str(),
         name.c_str(),
         initialize_from_name.c_str(),
         name.c_str());
   }
   //  else
   //  Parent defined and found:
   int parent_status = time_manager_init->get_status (parent_index);
   //  1.verify that this is not a loop
   if (parent_status == -1) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "In defining the tree for initializing the time-type, an infinite loop\n"
         "was encountered with the pre-defined 'time.time_XXX.initialize_from'\n"
         "values. Time-type '%s' depends on time-type '%s', and vice-versa. \n",
         initialize_from_name.c_str(),
         name.c_str());
   }

   // 2.check that a converter exists to do this task
   conv_index = parent_index * time_manager->num_types + index;
   if (time_manager_init->get_conv_dir_init (conv_index) == 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
         "An attempt was made to define time-type '%s'"
         " as the source for initializing '%s', but no converter was provided"
         " to convert from %s to %s at initialization.  \n",
         initialize_from_name.c_str(),
         name.c_str(),
         initialize_from_name.c_str(),
         name.c_str());
   }

   // 3.if the parent is not in the tree, add it (recursive addition).
   if (time_manager_init->get_status (parent_index) == 0) {
      time_manager->get_time_ptr (parent_index)->
      add_type_initialize (seeking_status, time_manager_init);
   }

   //  Not an else!  status[parent_index] could have changed from 0 to >0
   //  as a result of the call to add_type_initialize.
   //  Get here when one of:
   // 1. parent defined and in tree.  status[parent_index] > 0
   //    add child to tree
   // 2. parent defined, but tree broke higher up.  status[parent_index] = 0
   //    reset status[index] to 0 (indicating failed placement) and exit
   if (parent_status > 0) {
      time_manager_init->increment_status (index, parent_index);
   }
   else {
      time_manager_init->set_status (index, 0); // child cannot be added to the tree
   }

   return;
}



/**
 * Calculate Gregorian calendar date and 24-hour clock representation
 * from Truncated Julian date/time.
 *
 * \par Assumptions and Limitations
 *  - Coverage is from March 1, 1600 onward.
 *  - Produces a time in 24-hour clock format.
 *  - Assumes that the values year, month, day, hour, minute, second, and
 *      truncated_julian_time are all present in the same class.
 */
void
TimeStandard::calculate_calendar_values (
   void)
{
   int     julian_day, /* -- integral part of julian time */
           m,          /* -- preliminary month calculation */
           n_1,        /*  -- number of whole years */
           n_4,        /* -- number of whole 4-year periods */
           n_100,      /* -- number of whole 100-year periods */
           n_400,      /* -- number of whole 400-year periods */
           r_4,        /* -- number of years remaing after 4-year period */
           r_4a,       /* -- modified value of r_4 */
           r_100,      /* -- number of years remaing after 100-year period */
           r_400;      /* -- number of years remaing after 400-year period */

   double  temp;       /* -- temporary value */

   /*separate julian time into the integral and fractional parts */
   julian_day = static_cast<int> (trunc_julian_time);
   if (julian_day < 0) {
      julian_day -= 1;
   }

   /* minutes in fractional day */
   temp = 1440 * (trunc_julian_time - julian_day);
   /* whole minutes in fractional day */
   calendar_minute = static_cast<int> (temp);
   /* seconds in final fractional minute */
   calendar_second = 60 * (temp - calendar_minute);
   if (calendar_second > 60 - clock_resolution) {
      calendar_minute++;
      if (calendar_minute >= 1440) {
         julian_day++;
         calendar_minute -= 1440;
      }
      calendar_second = 0;
   }
   /* integer division, multiples of 60 in total minutes */
   calendar_hour = calendar_minute / 60;
   /* subtract away the hour multiples of 60. */
   calendar_minute -= (calendar_hour * 60);

   /* Note - could have done this with only one static_cast, but would have had
      to go from trunc_julian_time direct to minutes, which would cause a
      potential loss in precision. */

   /* convert the integral part into day, month,year format if day has changed */
   if (julian_day != prev_julian_day) {
      prev_julian_day = julian_day;
      // determine the number of whole 400-year periods since March 1, 1600, and
      // the number of days  since the "most recent" 400-year boundary
      julian_day += 134493; /* 134493 is the offset from March 1, 1600 to the
                              start of Truncated Julian Time */
      n_400       = julian_day / 146097; // There are 146097 days in 400 years.
      if (julian_day < 0) {
         n_400 -= 1;
      }
      r_400 = julian_day - 146097 * n_400 + 1;

      /* determine number of whole 100-year periods since the "most recent" 400-year
         boundary, and number of days since the "most recent" 100-year boundary */
      n_100 = static_cast<int> (r_400 / 36524.3);
      r_100 = r_400 - 36524 * n_100 - 1;

      /* ditto for 4-year periods */
      n_4 = r_100 / 1461;
      r_4 = r_100 - 1461 * n_4 + 1;

      /* and for 1-year periods, omitting remaining-days calculation */
      n_1 = static_cast<int> (r_4 / 365.3);

      /*  Modified r_4 for months calculation */
      r_4a = r_4 + 30 + 2 * n_1;

      /* m = (Number of whole months +1) since the most recent 4-year period
             (March=1, April=2, ... , February=12) */
      m = static_cast<int> (r_4a / 30.585);

/* Determine day of month, month number (Jan=1, etc.), and year number (AD) */
      calendar_day   = r_4a - static_cast<int> (30.585 * m);
      calendar_month = m + 2 - 12 * ((m + 1) / 12);
      calendar_year  = 1600 + 400 * n_400 + 100 * n_100 + 4 * n_4 +
                       ((m + 1) / 12);
   }

   return;
}



/**
 * Calls the function that converts the Julian-type representation
 * of time (dd.xxxx days) to a calendar representation.  Makes sure that
 * the time type on which it is called is up-to-date before doing so.
 *
 * \par Assumptions and Limitations
 *  - Derived times must have a parent; this should be defined by the
 *     user, or if not, already determined when the update_tree was built.
 * \param[in] simtime Simulation elapsed time, on the simulation clock\n Units: s
 */
void
TimeStandard::calendar_update (
   double simtime)
{
   // only process if it has not been done previously at this time
   if (!Numerical::compare_exact(simtime,last_calendar_update)) {
      // if the julian time needs updating, do that first.  Update all
      // types to ensure that dependencies are met.
      if (!Numerical::compare_exact(simtime,time_manager->simtime)) {
         time_manager->update (simtime);
      }
      last_calendar_update = simtime;
      calculate_calendar_values();
   }

   return;
}


/**
 * Calculate Truncated Julian date/time from Gregorian calendar date and
 * 24-hour clock representation.
 *
 * \par Assumptions and Limitations
 *  - Coverage s from March 1, 1600 onward.
 *  - Assumes that time is in 24-hour clock format; 1:00:00 pm cannot be read
 *      correctly, but 13:00:00 can.
 *  - Assumes that the values year, month, day, hour, minute, second, and
 *      truncated_julian_time are all present in the same class.
 */
void
TimeStandard::convert_from_calendar (
   void)
{
   int     m,    /*  adjusted month value */
           n_1,  /*  -- number of whole years */
           n_4,  /* -- number of whole 4-year periods */
           n_100, /* -- number of whole 100-year periods */
           n_400, /* -- number of whole 400-year periods */
           y,    /* -- adjusted year value */
           y_100, /* -- years remaining after subtracting off 100-yr multiples */
           y_400; /* -- years remaining after subtracting off 400-yr multiples */

   /* adjust the year to represent the number of whole years since March 1, 1600 */
   y = calendar_year - 1601 + (calendar_month + 9) / 12;

   /* Break down that number of years into a cumulative count of 400-year
      periods, 100-year periods, 4-year periods, and 1-year periods. */
   n_400 = y / 400;
   y_400 = y - 400 * n_400;

   n_100 = y_400 / 100;
   y_100 = y_400 - 100 * n_100;

   n_4 = y_100 / 4;
   n_1 = y_100 - 4 * n_4;

   /* Calculate the number of (partial) months since the end of February in a
      year that is divisible by 4 (even if it is not a leap year).
     (March = 1,13,25,37; April = 2,14,26,38; ... ; February = 12,24,36,48) */
   m = calendar_month - 2 + 12 * (1 - ((calendar_month + 9) / 12) + n_1);

   trunc_julian_time = calendar_day + floor (30.585 * m) - (2 * n_1) - 31 +
                       (1461 *
                        n_4) + (36524 * n_100) + (146097 * n_400) - 134493 +
                       (calendar_hour / 24.0) + (calendar_minute / 1440.0) +
                       (calendar_second / 86400.0);

   days    = trunc_julian_time - tjt_at_epoch;
   seconds = days * 86400;

   return;
}


/**
 * Each time type is initialized from its parent in the initialization
 * tree, except one.  In order to have an absolute reference time, one of
 * the time types must be defined ahead of time.  This is called the
 * initializer time. This function initializes the initializer time.
 *
 * \par Assumptions and Limitations
 *  - TimeDyn cannot be used as the initializer time.
 *  - Each time representation can have its own initializer function, or can
 *     inherit the one in TimeStandard.
 * \param[in] time_manager_init The TM initializer.
 */
//  General case
//  General case
void
TimeStandard::initialize_initializer_time (
   TimeManagerInit * time_manager_init)
{

   // ensure that the initializer time has not been defined as to-be-initialized
   // from some other time.  Default setting is "undefined".  If this has been
   // changed, exit immediately.
   if ( !initialize_from_name.empty() ) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Time-type '%s' has been flagged as the initializer, but \n"
         "'%s.initialize_from_name' has been defined as time-type '%s'.\n"
         "The initializer cannot be initialized by another type.\n"
         "This conflict cannot be resolved automatically. \n",
         name.c_str(), name.c_str(), initialize_from_name.c_str());
   }

   switch (time_manager_init->sim_start_format) {
   case TimeEnum::undefined:
      if (std::fpclassify(initializing_value) != FP_ZERO) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
            "The sim-start-format must be set to correctly interpret the "
            "initializing value.\n");
      }
      //   else
      if ((std::fpclassify(seconds) != FP_ZERO) && (std::fpclassify(days) != FP_ZERO) && (!Numerical::compare_exact(seconds,(days * 86400)))) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::redundancy_error, "\n"
            "Inconsistent initial values are set for the initializer time.\n");
      }
      // else
      if (std::fpclassify(calendar_year + calendar_month + calendar_day + calendar_minute +
           calendar_hour + calendar_second) == FP_ZERO) {
         if ((std::fpclassify(seconds) == FP_ZERO) && (std::fpclassify(days) == FP_ZERO)) {
            MessageHandler::fail (
               __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
               "No valid data for the simulation start time.\n");
         }
         // else
         if (std::fpclassify(seconds) == FP_ZERO) { // secs=0, cal=0, days !=0
            seconds = days * 86400;
         }
         else { // secs !=0, cal =0, days =0 || secs / 86400
            days = seconds / 86400;
         }
         trunc_julian_time = tjt_at_epoch + days;
         break;
      }
      else {  // calendar values set
         if ((std::fpclassify(seconds) != FP_ZERO) || (std::fpclassify(days) != FP_ZERO)) {
            MessageHandler::fail (
               __FILE__, __LINE__, TimeMessages::redundancy_error, "\n"
               "Calendar values and decimal values are defined.  Unsure how to "
               "proceed without a sim-start-format definition.\n");
         }
         // else fall through to calendar
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
            "A time format for initialization was not set. \n"
            "It appears that a calendar time was defined, \n"
            "initialization will be based on that.\n");
      }
      /* fall through */ 
   case TimeEnum::calendar: //calendar appears defined
      if (std::fpclassify(calendar_year + calendar_month + calendar_day + calendar_minute +
           calendar_hour + calendar_second) == FP_ZERO) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "No valid data for the simulation start time. \n"
            "Calendar values are all zero, "
            "indicating that the calendar has probably not been defined.  \n");
      }
      convert_from_calendar();
      days    = trunc_julian_time - tjt_at_epoch;
      seconds = days * 86400;
      break;
   case TimeEnum::Julian: case TimeEnum::julian:
      trunc_julian_time = initializing_value - 2440000.5;
      // 2440000.5 is the difference between Julian and Truncated Julian values.
      days    = trunc_julian_time - tjt_at_epoch;
      seconds = days * 86400;
      break;
   case TimeEnum::modified_julian:
      trunc_julian_time = initializing_value - 40000;
      // 40000 is the difference between mod-jul and truncated Julian values.
      days    = trunc_julian_time - tjt_at_epoch;
      seconds = days * 86400;
      break;
   case TimeEnum::truncated_julian:
      trunc_julian_time = initializing_value;
      days              = trunc_julian_time - tjt_at_epoch;
      seconds           = days * 86400;
      break;
   case TimeEnum::seconds_since_epoch:
      seconds           = initializing_value;
      days              = seconds / 86400;
      trunc_julian_time = tjt_at_epoch + days;
      break;
   case TimeEnum::days_since_epoch:
      days              = initializing_value;
      seconds           = days * 86400;
      trunc_julian_time = tjt_at_epoch + days;
      break;
   case TimeEnum::clock:
      //TODO: define behavior for this case
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
         "This behavior is currently undefined.\n");
      break;
   default:
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
         "Unknown Time format, cannot initialize.\n");
      break;

   }
   if (trunc_julian_time <= 0 && send_warning_pre_1968) {
      MessageHandler::warn (
         __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
         "The initialized value for %s.trunc_julian_time is %f. \n"
         "Values < 0 are acceptable for early-history simulations "
         "(pre- May 24, 1968),\nbut are often the result "
         "of improperly setting the desired simulation start values in "
         "the input file.\nSimulation will continue with this value.\n",
         name.c_str(), trunc_julian_time);
   }
   initialized   = true;
   initial_value = seconds;

   return;
}



/**
 * Initialize a time type from its parent on the initialization tree
 *
 * \par Assumptions and Limitations
 *  - More than 1 time-type defined, otherwise this is not called.
 * \param[in] time_manager_init The TM initializer.
 */
void
TimeStandard::initialize_from_parent (
   TimeManagerInit * time_manager_init)
{
   int parent_index = time_manager->time_lookup (initialize_from_name);
   /* -- index-value of parent time-type */

   //  Note parent_index must be defined (i.e. >=0) or the code would have
   //  already failed in create_init_tree.  But just in case:
   if ((parent_index < 0) ||
       (parent_index > time_manager->num_types)) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::initialization_error, "\n"
         "undetermined error in building the initialization tree.");
   }

   JeodBaseTime * parent_ptr = time_manager->get_time_ptr (parent_index); /* --
                                                pointer to parent time-type  */

   // If the parent time has not been initialized, initialize it (recursive loop)
   // The only type without a parent is the initializer, and that is not
   // initialized from this routine.
   if (!parent_ptr->is_initialized()) {
      parent_ptr->initialize_from_parent(time_manager_init);
   }

   int conv_index = parent_index * time_manager->num_types + index; /* --
                   converter index in TimeManagerInit converter registry */

   // parent is now initialized.  Make sure the converter is also initialized.
   // conv_index is where to look in the TimeManagerInit table for a converter
   // between the parent and the child.
   //  The TimeManagerInit table gives the location, in the TimeManager vector
   // that was loaded at the S_define level, of the pointer to the converter
   int conv_dir = time_manager_init->get_conv_dir_init (conv_index);
   if (conv_dir == 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
         "Error in initializing %s from %s.  \n"
         "Converter from %s to %s has not been registered for initialization.\n",
         name.c_str(),
         initialize_from_name.c_str(),
         initialize_from_name.c_str(),
         name.c_str());
   }
   int conv_ptr_index = time_manager_init->get_conv_ptr_index (
      conv_index);
   TimeConverter * converter_ptr =
      time_manager->get_converter_ptr (conv_ptr_index);
   if (!converter_ptr->is_initialized()) {
      converter_ptr->initialize (parent_ptr,
                                 this,
                                 conv_dir);
   }

   if (conv_dir == 1) {
      converter_ptr->convert_a_to_b();
   }
   else if (conv_dir == -1) {
      converter_ptr->convert_b_to_a();
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::memory_error, "\n"
         "Error in initializing %s from %s.  \n"
         "Conv_dir for the converter has an invalid value (<-1 or > 1)",
         name.c_str(),
         initialize_from_name.c_str());
   }
   initialized   = true;
   initial_value = seconds;
   return;
}


/**
 * Generate the number of seconds elapsed this year
 *
 * \par Assumptions and Limitations
 *  - Relies on the accuracy of the JEOD2.0 calendar.
 * @return Current second of year.
 */
double
TimeStandard::seconds_of_year (
   void)
{
   if (!Numerical::compare_exact(last_calendar_update,time_manager->simtime)) {
      calculate_calendar_values();
      last_calendar_update = time_manager->simtime;
   }

   if (calendar_year == year_of_last_soy) { // year has not changed since last update
      return (seconds - seconds_at_year_start);
   }
   else if (((calendar_year - year_of_last_soy) == 1) &&
            (calendar_month <= 2)) {
      // year has ticked over, but less than 2 full months into new year
      double seconds_of_year_value =
         ((calendar_month - 1) * 31 + (calendar_day - 1)) * 86400 +
         calendar_hour * 3600 + calendar_minute * 60 + calendar_second;
      seconds_at_year_start = seconds - seconds_of_year_value;
      year_of_last_soy      = calendar_year;
      return seconds_of_year_value;
   }

   // Get here is this is the first time calculating seconds_of_year, or if year
   // has ticked over backward, or if it is on or after March 1 in the year
   // immediately following the last seconds_of_year calculation.
   int temp_c_month     = calendar_month;
   int temp_c_day       = calendar_day;
   int temp_c_hour      = calendar_hour;
   int temp_c_minute    = calendar_minute;
   double temp_c_second = calendar_second;
   double temp_tjt      = trunc_julian_time;
   double temp_days     = days;
   double temp_seconds  = seconds;

   // set the time to be at year-start
   calendar_month  = 1;
   calendar_day    = 1;
   calendar_hour   = 0;
   calendar_minute = 0;
   calendar_second = 0.0;

   // convert to decimal, save off appropriate values
   convert_from_calendar();
   seconds_at_year_start = seconds;
   year_of_last_soy      = calendar_year;

   // revert to proper time
   calendar_month    = temp_c_month;
   calendar_day      = temp_c_day;
   calendar_hour     = temp_c_hour;
   calendar_minute   = temp_c_minute;
   calendar_second   = temp_c_second;
   trunc_julian_time = temp_tjt;
   days              = temp_days;
   seconds           = temp_seconds;

   return (seconds - seconds_at_year_start);
}

/**
 * Destroy a TimeStandard
 */
TimeStandard::~TimeStandard (
   void)
{
   // Default
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
