/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_met.cc
 * Define member functions for Mission Elapsed Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_met.cc)
   (time.cc)
   (time_ude.cc)
   (utils/named_item/src/named_item.cc))


******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/time_met.hh"

// #include "../include/time_manager.hh"
// #include "../include/time_enum.hh"
// #include "../include/time_converter_std_ude.hh"
// #include "../include/time_converter_dyn_ude.hh"
// #include "../include/time_messages.hh"

/**
 * Construct a Time_MET
 */
// #include "../include/time_enum.hh"
// #include "../include/time_converter_std_ude.hh"
// #include "../include/time_converter_dyn_ude.hh"
// #include "../include/time_messages.hh"

//! Namespace jeod
namespace jeod
{

/******************************************************************************
 Function: Time_MET::Time_MET
 Purpose: (Construct a Time_MET)
 Class:   (N/A)
******************************************************************************/
TimeMET::TimeMET()
{
    name = "MET";
}

/**
 * Updates to current time
 */
void TimeMET::update()
{
    if(hold == previous_hold)
    {
        if(!hold)
        { // normal operation
            JeodBaseTime::update();
        }
        // else no action needed, still in hold.
    }
    else
    {
        previous_hold = hold; // record setting
        if(!hold)
        { // transitioned to hold
            update_converter_ptr->reset_a_to_b_offset();
        }
        // else transitioned to hold, don't do anything.
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
