/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_tai_utc.cc
 * Converts between International Atomic Time and Coordinated Universal Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter_tai_utc.cc)
   (time.cc)
   (time_standard.cc)
   (time_tai.cc)
   (time_utc.cc)
   (time_converter.cc)
   (time_messages.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))


******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/time_converter_tai_utc.hh"
#include "../include/time_manager.hh"
#include "../include/time_messages.hh"
#include "../include/time_tai.hh"
#include "../include/time_utc.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Construct a TimeConverter_TAI_UTC
 */
TimeConverter_TAI_UTC::TimeConverter_TAI_UTC()
{
    a_name = "TAI";
    b_name = "UTC";
    valid_directions = A_TO_B | B_TO_A_INIT;
}

/**
 * Initialize the converter.
 * \param[in] parent_ptr Time used to initialize the converter
 * \param[in] child_ptr Other Time used to initialize the converter
 * \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 */
void TimeConverter_TAI_UTC::initialize(JeodBaseTime * parent_ptr, JeodBaseTime * child_ptr, const int int_dir)
{
    verify_setup(parent_ptr, child_ptr, int_dir);

    if(int_dir == 1)
    {
        tai_ptr = dynamic_cast<TimeTAI *>(parent_ptr);
        utc_ptr = dynamic_cast<TimeUTC *>(child_ptr);
        //  lookup table is calibrated in UTC,
        if(utc_ptr->is_initialized())
        {
            initialize_leap_second();
        }
        else
        {
            // start with TAI as first iteration
            utc_ptr->trunc_julian_time = tai_ptr->trunc_julian_time;
            initialize_leap_second();

            // Now adjust TAI as second iteration.
            utc_ptr->trunc_julian_time = tai_ptr->trunc_julian_time + a_to_b_offset;

            /*  If the leap-second correction takes UTC back before the time of the
                last leap second, and if it does so by more than the leap second
                itself, need to reinitialize */
            if((index > 0) &&
               ((when_vec[index] - utc_ptr->trunc_julian_time) > ((val_vec[index] - val_vec[index - 1]) / 86400.0)))
            {
                initialize_leap_second();
            }
        }
    }
    else if(int_dir == -1)
    {
        tai_ptr = dynamic_cast<TimeTAI *>(child_ptr);
        utc_ptr = dynamic_cast<TimeUTC *>(parent_ptr);
        initialize_leap_second(); //  utc must be initialized because it is parent
    }
    else
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::initialization_error,
                             "\n"
                             "Illegal value of int_dir in TAI->UTC initializer");
    }
    initialized = true;
}

/**
 * The conversion from Atomic Time (TAI) to Universal Time (UTC)
 * involves the addition of leap seconds. The number of leap seconds at any
 * given (historical) time is provided in a table. This function initializes
 * that table and sets the preliminary values.
 *
 * \par Assumptions and Limitations
 *  - The table does not go into the future.
 */
void TimeConverter_TAI_UTC::initialize_leap_second()
{
    /* if the user inputs their own value for a_to_b_offset, make sure that it
    does not get overridden. */
    if(override_data_table)
    {
        utc_ptr->true_utc = false;
        a_to_b_offset = -leap_sec_override_val / 86400.0;
        return;
    }

    if((when_vec == nullptr) || (val_vec == nullptr))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             TimeMessages::invalid_setup_error,
                             "\n"
                             "The TAI to UTC conversion function requires a"
                             " data lookup table, or the flag 'override_data_table' to be set"
                             " but none has been provided and the flag remains 'false'.\n");
        return;
    }

    double trunc_julian_time = utc_ptr->trunc_julian_time; /* --
                                       local value of UTC time in TJT format */
    off_table_end = false;

    if(trunc_julian_time < when_vec[0])
    {
        /* Julian date is less than initial date in table, set leap_second to 0 */
        if(tai_ptr->time_manager->dyn_time.scale_factor >= 0)
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::invalid_data_error,
                                   "\n"
                                   "\nThe current time precedes the first entry in the"
                                   " look-up table for leap-second values. \n"
                                   "Leap_second value is being set to the first value in the table"
                                   " (%i s) .\n"
                                   "Simulation will continue to run and will continue to monitor\n"
                                   "time for future crossing into the tabulated range.\n",
                                   val_vec[0]);
            index = -1; // because update will increment to index+1, i.e. 0
            next_when = when_vec[0];
            prev_when = trunc_julian_time - (when_vec[0] - trunc_julian_time);
            // something less than trunc_julian_time
        }
        else
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::invalid_data_error,
                                   "\n"
                                   "\nThe current time precedes the first entry in the"
                                   " look-up table for leap-second values. \n"
                                   "Leap_second value is being set to the first value in the table"
                                   " (%i s) .\n"
                                   "Simulation will continue to run, and this value will be\n"
                                   "maintained unless the time-direction changes.\n",
                                   val_vec[0]);
            off_table_end = true;
            index = 0;
        }
        a_to_b_offset = -val_vec[0] / 86400.0;
    }
    else if(trunc_julian_time >= when_vec[last_index])
    {
        /* Julian date is beyond the table, set leap-second to last known value */
        index = last_index;
        if(tai_ptr->time_manager->dyn_time.scale_factor >= 0)
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::invalid_data_error,
                                   "\n"
                                   "\nThe current time follows the last entry in the"
                                   " look-up table for leap-second values. \n"
                                   "Leap_second value is being set to the last value in the table"
                                   " (%i s) .\n"
                                   "Simulation will continue to run, and this value will be\n"
                                   "maintained unless the time-direction changes.\n",
                                   val_vec[last_index]);
            off_table_end = true;
        }
        else
        {
            MessageHandler::inform(__FILE__,
                                   __LINE__,
                                   TimeMessages::invalid_data_error,
                                   "\n"
                                   "\nThe current time follows the last entry in the"
                                   " look-up table for leap-second values. \n"
                                   "Leap_second value is being set to the last value in the table"
                                   " (%i s) .\n"
                                   "Simulation will continue to run and will continue to monitor\n"
                                   "time for future crossing into the tabulated range.\n",
                                   val_vec[last_index]);
            prev_when = when_vec[last_index];
            next_when = trunc_julian_time + (trunc_julian_time - when_vec[last_index]);
            // some value more than trunc_julian_time
        }
        a_to_b_offset = -val_vec[last_index] / 86400.0;
    }
    else
    {
        /* Find the last entry that comes before the current time (trunc_julian_time)
           Highest value that can be reached is index = last_index -1
           because trunc_julian_time < leap_when(last_leap_index) by earlier test. */
        for(index = 0; trunc_julian_time >= when_vec[index + 1]; ++index)
        {
            ;
        }
        prev_when = when_vec[index];
        next_when = when_vec[index + 1];
        /* and take the number of leap seconds recorded in the previous entry (i.e.
           the last entry before the current time) */
        a_to_b_offset = -val_vec[index] / 86400.0;
    }
}

/**
 * Convert from TimeTAI to TimeUTC.
 *
 * \par Assumptions and Limitations
 *  - Time class MET is based on time class TAI, and counts the
 *                 elapsed TAI time only
 */
void TimeConverter_TAI_UTC::convert_a_to_b()
{
    utc_ptr->trunc_julian_time = tai_ptr->trunc_julian_time + a_to_b_offset;

    //  check for potential updates to a_to_b_offset:
    if(!off_table_end && utc_ptr->true_utc)
    {
        // for conventional (forward-time) simulations.
        if(tai_ptr->time_manager->dyn_time.scale_factor >= 0)
        {
            //"while" is used because updates need not be done regularly, may have
            // to catch up with accumulation of points. */
            while(utc_ptr->trunc_julian_time >= next_when)
            {
                index++;
                /*  assign these here.  It is redundant if the loop runs more than once,
                    but assigning after the loop would be redundant if the loop was not
                    entered at all.  The latter case is more likely, so assigning here
                    is more efficient. */
                a_to_b_offset = -val_vec[index] / 86400.0;
                utc_ptr->set_time_by_trunc_julian(tai_ptr->trunc_julian_time + a_to_b_offset);
                // utc_ptr->trunc_julian_time = tai_ptr->trunc_julian_time + a_to_b_offset;

                prev_when = next_when;

                if(index < last_index)
                {
                    next_when = when_vec[index + 1];
                }
                else
                { // run out of table!
                    MessageHandler::inform(__FILE__,
                                           __LINE__,
                                           TimeMessages::invalid_data_error,
                                           "\n"
                                           " Current time is beyond the range covered by the leap-second table.\n"
                                           " The difference between TAI and UTC is being set to the last value \n"
                                           " in the table.  The simulation will continue to run with this value. "
                                           " \n");
                    off_table_end = true;
                    break;
                }
            }
        }
        else
        {
            // for reverse-time simulations.
            while(utc_ptr->trunc_julian_time < prev_when)
            {
                index--;
                next_when = prev_when;

                if(index >= 0)
                {
                    a_to_b_offset = -val_vec[index] / 86400.0;
                    utc_ptr->set_time_by_trunc_julian(tai_ptr->trunc_julian_time + a_to_b_offset);
                    prev_when = when_vec[index];
                }
                else
                { // run out of table!
                    MessageHandler::inform(__FILE__,
                                           __LINE__,
                                           TimeMessages::invalid_data_error,
                                           "\n"
                                           "Current time is beyond the range covered by the leap-second table.\n"
                                           "The difference between TAI and UTC is being set to the first value \n"
                                           "in the table.  The simulation will continue to run with this value."
                                           " \n");
                    off_table_end = true;
                    a_to_b_offset = 0;
                    utc_ptr->set_time_by_trunc_julian(tai_ptr->trunc_julian_time + a_to_b_offset);
                    break;
                }
            }
        }
    }
    /* if no update to leap seconds was needed, do not need to change
       trunc_julian_time from that calculated earlier.
       if an update was needed, it has already been recalculated.
       Either way, do not need to do it again here.  Done. */
    utc_ptr->set_time_by_trunc_julian(tai_ptr->trunc_julian_time + a_to_b_offset);

    //  utc_ptr->days = utc_ptr->trunc_julian_time - utc_ptr->tjt_at_epoch;
    //  utc_ptr->seconds = utc_ptr->days * 86400;
}

/**
 * Convert from TimeUTC to TimeTAI.
 */
void TimeConverter_TAI_UTC::convert_b_to_a()
{
    if(!off_table_end && utc_ptr->true_utc)
    {
        // for conventional (forward-time) simulations.
        if(tai_ptr->time_manager->dyn_time.scale_factor >= 0)
        {
            // "while" is used because updates need not be done regularly, may have
            // to catch up with accumulation of points. */
            while(utc_ptr->trunc_julian_time >= next_when)
            {
                index++;
                a_to_b_offset = -val_vec[index] / 86400.0;
                prev_when = next_when;

                if(index < last_index)
                {
                    next_when = when_vec[index + 1];
                }
                else
                { // run out of table!
                    index = last_index;
                    MessageHandler::inform(__FILE__,
                                           __LINE__,
                                           TimeMessages::invalid_data_error,
                                           "\n"
                                           "Current time is beyond the range covered by the leap-second table.\n"
                                           "The difference between TAI and UTC is being set to the last value \n"
                                           "in the table.  The simulation will continue to run with this value."
                                           " \n");
                    off_table_end = true;
                    break;
                }
            }
        }
        else
        {
            // for reverse-time simulations.
            while(utc_ptr->trunc_julian_time < prev_when)
            {
                index--;
                next_when = prev_when;

                if(index >= 0)
                {
                    a_to_b_offset = -val_vec[index] / 86400.0;
                    prev_when = when_vec[index];
                }
                else
                { // run out of table!
                    MessageHandler::inform(__FILE__,
                                           __LINE__,
                                           TimeMessages::invalid_data_error,
                                           "\n"
                                           "Current time is beyond the range covered by the leap-second table.\n"
                                           "The difference between TAI and UTC is being set to the first value \n"
                                           "in the table.  The simulation will continue to run with this value."
                                           " \n");
                    off_table_end = true;
                    a_to_b_offset = 0;
                    break;
                }
            }
        }
    }

    tai_ptr->set_time_by_trunc_julian(utc_ptr->trunc_julian_time - a_to_b_offset);
}

/**
 * Used when time reverses direction.
 * Checks whether the table lookup function is using input values that
 *            are outside the scope of the table, and sets the flags appropriately
 */
void TimeConverter_TAI_UTC::verify_table_lookup_ends()
{
    if(!utc_ptr->true_utc)
    { // table is not monitored
        return;
    }

    // if now going forward in time
    if(tai_ptr->time_manager->dyn_time.scale_factor >= 0)
    {
        // and past the end of the table, past the table
        if(index >= last_index)
        {
            off_table_end = true;
        }
        /* or if before the beginning of the table, no longer past the table. */
        /* Change "prev_when" to be before "now", or the converter will
           automatically revert to being off the table */
        else if(index < 0)
        {
            prev_when = utc_ptr->trunc_julian_time - 1;
            next_when = when_vec[0];
            index = -1;
            off_table_end = false;
        }
    }
    else
    { // reverse
        if(index >= last_index)
        {
            next_when = utc_ptr->trunc_julian_time + 1;
            prev_when = when_vec[last_index];
            index = last_index;
            off_table_end = false;
        }
        else if(index < 0)
        {
            off_table_end = true;
        }
    }
}

/**
 * Destroy a TimeConverter_TAI_UTC
 */
TimeConverter_TAI_UTC::~TimeConverter_TAI_UTC()
{
    JEOD_DELETE_ARRAY(when_vec);
    JEOD_DELETE_ARRAY(val_vec);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
