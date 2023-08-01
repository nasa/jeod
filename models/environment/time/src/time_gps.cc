/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_gps.cc
 * Define member functions for the clock associated with the Global Positioning
 * System.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_gps.cc)
   (time.cc)
   (time_messages.cc)
   (time_standard.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))

 
******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_gps.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a Time_GPS
 */
TimeGPS::TimeGPS (
   void)
{
   name = "GPS";

   seconds_of_day  = 0.0;
   seconds_of_week = 0.0;
   day_of_week     = 0;
   rollover_count  = 0;
   week            = 0;
   rollover_count_13_bit  = 0;
   week_13_bit            = 0;
   set_epoch();
}


/**
 * Sets the epoch for GPS time
 */
void
TimeGPS::set_epoch(
   void)
{
   // Calculate the epoch for GPS time by adding 19 leap seconds to the UTC
   // value of TJT, 4244.0, at the GPS epoch (midnight on January 5/6, 1980).
   tjt_at_epoch    = 4244.0 + 19.0 / 86400.0;

   return;
}

/**
 * Protection against inheriting nonsense function
 *
 * \par Assumptions and Limitations
 *  - GPS does not have a conventional calendar
 */
void
TimeGPS::convert_from_calendar (
   void)
{
   MessageHandler::fail (
      __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
      "Attempted to use a calendar for GPS time, but GPS time does not\n"
      "have a conventional calendar.\n");
   return;
}


/**
 * Protection against inheriting nonsense function
 *
 * \par Assumptions and Limitations
 *  - GPS does not have a conventional calendar
 */
void
TimeGPS::calculate_calendar_values (
   void)
{
   MessageHandler::fail (
      __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
      "Attempted to calculate a calendar for GPS time, but GPS time does not\n"
      "have a conventional calendar.\n");
   return;
}

/**
 * Given a value of seconds, propagate to other reps.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_seconds new value for seconds\n Units: s
 */
void
TimeGPS::set_time_by_seconds (
   const double new_seconds)

{
   TimeStandard::set_time_by_seconds (new_seconds);
   // Split time into day number and seconds of day.
   int gps_time_int = static_cast<int> (days);
   seconds_of_day = (days - gps_time_int) * 86400.0;

   // GPS time rolls over every 1024 weeks, or every 7168 days.
   rollover_count = gps_time_int / 7168;
   // In newer systems, GPS time rolls over every 8192 weeks, or 57344 days.
   rollover_count_13_bit = gps_time_int / 57344;
   week_13_bit = (gps_time_int-57344*rollover_count_13_bit)/7;
   gps_time_int  -= rollover_count * 7168;
   week           = gps_time_int / 7; // weeks since rollover

   day_of_week =
      gps_time_int - week * 7;   /* number of full days in this
                                             (partial) week */
   seconds_of_week =
      day_of_week * 86400 + seconds_of_day;   // convert to seconds

   return;

}

/**
 * Given a value of days, propagate to other values.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_days new value for days\n Units: day
 */
void
TimeGPS::set_time_by_days (
   const double new_days)

{
   set_time_by_seconds (new_days * 86400.0);
   return;
}

/**
 * TJT does not function in GPS.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] new_tjt new value for Truncated Julian Time\n Units: day
 */
void
TimeGPS::set_time_by_trunc_julian (
   const double new_tjt)

{
   TimeStandard::set_time_by_trunc_julian (new_tjt);
   set_time_by_seconds (seconds);
   return;
}



/**
 * Destroy a TimeGPS
 */
TimeGPS::~TimeGPS (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
