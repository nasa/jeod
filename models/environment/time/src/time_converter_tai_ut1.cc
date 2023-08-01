/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_tai_ut1.cc
 * Converts between International Atomic Time and Universal Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter_tai_ut1.cc)
   (time.cc)
   (time_standard.cc)
   (time_tai.cc)
   (time_ut1.cc)
   (time_converter.cc)
   (time_messages.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))

 
******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_converter_tai_ut1.hh"
#include "../include/time_tai.hh"
#include "../include/time_ut1.hh"
#include "../include/time_manager.hh"
#include "../include/time_messages.hh"

//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_TAI_UT1
 */
TimeConverter_TAI_UT1::TimeConverter_TAI_UT1 (
   void)
{
   a_name                = "TAI";
   b_name                = "UT1";
   valid_directions = ANY_DIRECTION;

   override_data_table = false;

   tai_ptr                 = nullptr;
   ut1_ptr                 = nullptr;
   tai_to_ut1_override_val = 0.0;
   last_index              = 0;
   index                   = -1;
   val_vec                 = nullptr;
   when_vec                = nullptr;
   prev_when               = 0.0;
   prev_value              = 0.0;
   next_when               = 0.0;
   next_value              = 0.0;
   gradient                = 0.0;
   off_table_end           = false;
}


/**
 * Initialize the converter.
 *
 * \par Assumptions and Limitations
 *  - None
 * \param[in] parent_ptr Time used to initialize the converter
 * \param[in] child_ptr Other Time used to initialize the converter
 * \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 */
void
TimeConverter_TAI_UT1::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{
   verify_setup (parent_ptr, child_ptr, int_dir);

   /*  The time in the lookup table is TAI.  Initializing from TAI is preferred,
       but possible from UT1. */
   if (int_dir == 1) {
      tai_ptr = dynamic_cast<TimeTAI *> (parent_ptr);
      ut1_ptr = dynamic_cast<TimeUT1 *> (child_ptr);
      initialize_tai_to_ut1();
   }
   else if (int_dir == -1) {
      tai_ptr = dynamic_cast<TimeTAI *> (child_ptr);
      ut1_ptr = dynamic_cast<TimeUT1 *> (parent_ptr);
      //  start with TAI = UT1 as first iteration
      tai_ptr->trunc_julian_time = ut1_ptr->trunc_julian_time;
      initialize_tai_to_ut1();

      /* adjust TAI accordingly.  If TAI is in a different "box", re-run the
         initialization. */
      tai_ptr->trunc_julian_time = ut1_ptr->trunc_julian_time - a_to_b_offset;
      if ((tai_ptr->trunc_julian_time > next_when) ||
          (tai_ptr->trunc_julian_time < prev_when)) {
         initialize_tai_to_ut1();
      }
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::initialization_error, "\n"
         "Illegal value of int_dir in TAI->UT1 initializer");
   }
   initialized = true;

   return;
}


/**
 * The conversion from Atomic Time (TAI) to Universal Time (UT1)
 * involves the addition of value that is a continuous function of TAI.
 * That value is tabulated at regular points of TAI.  This function
 * initializes that table and sets the preliminary values.
 *
 * \par Assumptions and Limitations
 *  - The table does not go into the future.
 */
void
TimeConverter_TAI_UT1::initialize_tai_to_ut1 (
   void)
{
   /* if the user inputs their own value for a_to_b_offset, make sure that it
      does not get overridden. */
   if (override_data_table) {
      ut1_ptr->true_ut1 = false;
      a_to_b_offset     = tai_to_ut1_override_val / 86400;
      return;
   }

   if ((when_vec == nullptr) || (val_vec == nullptr)) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
         "The TAI to UT1 conversion function requires a"
         " data lookup table, but none has been provided.\n");
      return;
   }

   double tai_time = tai_ptr->trunc_julian_time; /* day local value of TAI time */
   off_table_end = false;

   if (tai_time < when_vec[0]) {
      // Julian date is less than initial date in table
      if (tai_ptr->time_manager->dyn_time.scale_factor >= 0) {
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "The current time precedes the first entry in the"
            " look-up table for TAI to UT1 conversions. \n"
            "The conversion value is being set to the first value in the table"
            " (%f s) .\n"
            "Simulation will continue to run and will continue to monitor\n"
            "time for future crossing into the tabulated range.\n",
            val_vec[0]);
         index      = -1; // because update will look at index+1, i.e. 0
         prev_when  = tai_time - 1;
         next_when  = when_vec[0];
         next_value = val_vec[0];
      }
      else {
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "The current time precedes the first entry in the"
            " look-up table for TAI to UT1 conversions. \n"
            "The conversion value is being set to the first value in the table"
            " (%f s) .\n"
            "Simulation will continue to run, and this value will be\n"
            "maintained unless the time-direction changes.\n",
            val_vec[0]);
         off_table_end = true;
         index         = 0;
      }
      a_to_b_offset = val_vec[0] / 86400.0;
   }
   else if (tai_time >= when_vec[last_index]) {
      // Julian date is beyond the table
      index = last_index;
      if (tai_ptr->time_manager->dyn_time.scale_factor >= 0) {
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "The current time follows the last entry in the"
            " look-up table for TAI to UT1 conversions. \n"
            "The conversion value is being set to the last value in the table"
            " (%f s) .\n"
            "Simulation will continue to run, and this value will be\n"
            "maintained unless the time-direction changes.\n",
            val_vec[last_index]);
         off_table_end = true;
      }
      else {
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "The current time follows the last entry in the"
            " look-up table for TAI to UT1 conversions. \n"
            "The conversion value is being set to the last value in the table"
            " (%f s) .\n"
            "Simulation will continue to run and will continue to monitor\n"
            "time for future crossing into the tabulated range.\n",
            val_vec[last_index]);
         prev_when  = when_vec[last_index];
         prev_value = val_vec[last_index];
         next_when  = tai_time + 1;
      }
      a_to_b_offset = val_vec[last_index] / 86400.0;
   }
   else {

      /* Find the last entry that comes before the current time (trunc_julian_time)
         Highest value that can be reached is index = last_index - 1
         because tai_time < when_vec[last_index] by earlier test. */
      for (index = 0; tai_time >= when_vec[index + 1]; ++index) {
         ;
      }
      prev_when     = when_vec[index];
      prev_value    = val_vec[index];
      next_when     = when_vec[index + 1];
      next_value    = val_vec[index + 1];
      gradient      = (next_value - prev_value) /  (next_when - prev_when);
      a_to_b_offset =
         (prev_value + (tai_time - prev_when) * gradient) / 86400.0;
   }

   return;
}


/**
 * Convert from TimeTAI to TimeUT1.
 *
 * \par Assumptions and Limitations
 *  - Time class MET is based on time class TAI, and counts the elapsed
 *                TAI time only
 */
void
TimeConverter_TAI_UT1::convert_a_to_b (
   void)
{
   /*  "when" is in days
       a_to_b_offset is in seconds
       gradient is in sec / day. */

   double tai_time = tai_ptr->trunc_julian_time; // -- local value of TAI time

   if (off_table_end || !ut1_ptr->true_ut1) { // a_to_b_offset does not change
      ut1_ptr->set_time_by_trunc_julian (tai_ptr->trunc_julian_time  +
                                         a_to_b_offset);
      // ut1_ptr->trunc_julian_time = tai_time + a_to_b_offset;
      // ut1_ptr->days = ut1_ptr->trunc_julian_time - ut1_ptr->tjt_at_epoch;
      // ut1_ptr->seconds = ut1_ptr->days * 86400;
      return;
   } // else
     // for conventional (forward-time) simulations:

   /* "while" is used because updates need not be done regularly, may have to
      catch up with accumulation of points. */
   while (tai_time > next_when) {
      index++;
      /* Doing a full assignment at each scan because in most cases, this loop is
         skipped; a partial assignment (index and next_when
         only) would require the assignment of the other values after the
         while-loop, regardless of whether the while-loop was entered. */
      prev_value = next_value;
      prev_when  = next_when;

      if (index < last_index) {
         next_value = val_vec[index + 1];
         next_when  = when_vec[index + 1];
         gradient   = (next_value - prev_value) /  (next_when - prev_when);
      }
      else {
         /* Julian date is beyond the table, set to last value */
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "Current time is beyond the range covered by the TAI-UT1 table.\n"
            "The difference between TAI and UT1 is being set to the last value \n"
            "in the table.  The simulation will continue to run with this value."
            "\n");
         off_table_end = true;
         gradient      = 0.0;
         break;
      }
   }
   /* do same for unconventional (reverse-time) sims.
      prev still references the table entry that precedes current real-time,
      even though it will be reached later in the sim (due to reverse
      direction). */
   while (tai_time < prev_when) {
      index--;
      next_value = prev_value;
      next_when  = prev_when;

      if (index >= 0) {
         prev_value = val_vec[index];
         prev_when  = when_vec[index];
         gradient   = (next_value - prev_value) /  (next_when - prev_when);
      }
      else {
         /* Julian date is less than initial date in table. */
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "Current time is beyond the range covered by the TAI-UT1 table.\n"
            "The difference between TAI and UT1 is being set to the first value\n"
            "in the table.  The simulation will continue to run with this value."
            "\n");
         off_table_end = true;
         gradient      = 0.0;
         break;
      }
   }
   a_to_b_offset = (prev_value + (tai_time - prev_when) * gradient) / 86400.0;

   ut1_ptr->set_time_by_trunc_julian (tai_ptr->trunc_julian_time  +
                                      a_to_b_offset);
   // ut1_ptr->trunc_julian_time = tai_time + a_to_b_offset;
   // ut1_ptr->days = ut1_ptr->trunc_julian_time - ut1_ptr->tjt_at_epoch;
   // ut1_ptr->seconds = ut1_ptr->days * 86400;

   return;
}


/**
 * Convert from TimeUT1 to TimeTAI.
 *
 * \par Assumptions and Limitations
 *  - Time class MET is based on time class TAI, and counts the elapsed
 *                TAI time only
 */
void
TimeConverter_TAI_UT1::convert_b_to_a (
   void)
{
   //  This function is going to look a lot like convert_a_to_b.
   double ut1_time = ut1_ptr->trunc_julian_time; // -- local value of UT1 time

   // update TAI using the old TAI_to_UT1 value

   if (off_table_end || !ut1_ptr->true_ut1) { // a_to_b_offset does not change
      tai_ptr->set_time_by_trunc_julian (ut1_ptr->trunc_julian_time  -
                                         a_to_b_offset);
      // tai_ptr->trunc_julian_time = ut1_time - a_to_b_offset;
      // tai_ptr->days = tai_ptr->trunc_julian_time - tai_ptr->tjt_at_epoch;
      // tai_ptr->seconds = tai_ptr->days * 86400;
      return;
   } // else
     // for conventional (forward-time) simulations:

   /* "while" is used because updates need not be done regularly, may have to
       catch up with accumulation of points. */
   while (ut1_time > next_when + (next_value / 86400.0)) {
      index++;
      /* Doing a full assignment at each scan because in most cases, this loop is
         skipped; a partial assignment (index and next_when only) would require
         the assignment of the other values after the while-loop, regardless of
         whether the while-loop was entered. */
      prev_value = next_value;
      prev_when  = next_when;

      if (index < last_index) {
         next_value = val_vec[index + 1];
         next_when  = when_vec[index + 1];
         gradient   = (next_value - prev_value) / (next_when - prev_when);
      }
      else {
         /* Julian date is beyond the table, set to last value */
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "Current time is beyond the range covered by the TAI-UT1 table.\n"
            "The difference between TAI and UT1 is being set to the last value\n"
            "the table.  The simulation will continue to run with this value."
            "\n");
         off_table_end = true;
         gradient      = 0.0;
         break;
      }
   }
   /* do same for unconventional (reverse-time) sims.
      prev still references the table entry that precedes current real-time,
      even though it will be reached later in the sim (due to reverse
      direction). */
   while (ut1_time < prev_when + (prev_value / 86400.0)) {
      index--;
      next_value = prev_value;
      next_when  = prev_when;

      if (index >= 0) {
         prev_value = val_vec[index];
         prev_when  = when_vec[index];
         gradient   = (next_value - prev_value) / (next_when - prev_when);
      }
      else { /* Julian date is less than initial date in table. */
         MessageHandler::inform (
            __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
            "Current time is beyond the range covered by the TAI-UT1 table.\n"
            "The difference between TAI and UT1 is being set to the first value\n"
            "in the table.  The simulation will continue to run with this value."
            "\n");
         off_table_end = true;
         gradient      = 0.0;
         break;
      }
   }
   a_to_b_offset = (prev_value + (ut1_time - prev_when) * gradient) /
                   (86400.0 + gradient);

   tai_ptr->set_time_by_trunc_julian (ut1_ptr->trunc_julian_time  -
                                      a_to_b_offset);
   // tai_ptr->trunc_julian_time = ut1_time - a_to_b_offset;
   // tai_ptr->days = tai_ptr->trunc_julian_time - tai_ptr->tjt_at_epoch;
   // tai_ptr->seconds = tai_ptr->days * 86400;


   return;
}


/**
 * Used when time reverses direction.
 * Checks whether the table lookup function is using input values that
 * are outside the scope of the table, and sets the flags appropriately
 *
 * \par Assumptions and Limitations
 *  - None
 */
void
TimeConverter_TAI_UT1::verify_table_lookup_ends (
   void)
{
   if (!ut1_ptr->true_ut1) { // table is not monitored
      return;
   }

   // if now going forward in time
   if (tai_ptr->time_manager->dyn_time.scale_factor >= 0) {
      // and past the end of the table, past the table
      if (index >= last_index) {
         off_table_end = true;
      }
      /* or if before the beginning of the table, no longer past the table. */
      /* Change "prev_when" to be before "now", or the converter will
         automatically revert to being off the table */
      else if (index < 0) {
         prev_when     = tai_ptr->trunc_julian_time - 1;
         next_when     = when_vec[0];
         index         = -1;
         off_table_end = false;
      }
   }
   else { // reverse
      if (index >= last_index) {
         next_when     = tai_ptr->trunc_julian_time + 1;
         prev_when     = when_vec[ last_index ];
         index         = last_index;
         off_table_end = false;
      }
      else if (index < 0) {
         off_table_end = true;
      }
   }
   return;
}



/**
 * Destroy a TimeConverter_TAI_UT1
 */
TimeConverter_TAI_UT1::~TimeConverter_TAI_UT1 (
   void)
{
   if ((when_vec != nullptr)  && (JEOD_IS_ALLOCATED (when_vec))) {
      JEOD_DELETE_ARRAY (when_vec);
   }
   if ((val_vec != nullptr)  && (JEOD_IS_ALLOCATED (val_vec))) {
      JEOD_DELETE_ARRAY (val_vec);
   }
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
