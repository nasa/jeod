/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_gmst.cc
 * Define member functions for Greenwich Mean Sidereal Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_gmst.o)
   (time.o)
   (time_messages.o)
   (time_standard.o)
   (utils/sim_interface/memory_interface.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o))

 
******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/time_gmst.hh"
#include "../include/time_messages.hh"



//! Namespace jeod
namespace jeod {

/**
 * Construct a Time_GMST
 */
TimeGMST::TimeGMST (
   void)
{
   name = "GMST";
}

/**
 * Protection against inheriting nonsense function
 *
 * \par Assumptions and Limitations
 *  - GMST does not have a conventional calendar
 */
void
TimeGMST::calculate_calendar_values (
   void)
{
   MessageHandler::fail (
      __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
      "Attempted to calculate a calendar for GMST time, but GMST time does not\n"
      "have a conventional calendar.\n");
   return;
}

/**
 * TJT does not function in GMST.
 *
 * \par Assumptions and Limitations
 *  - 86400 seconds = 1 day
 * \param[in] nonsense Any old invalid value
 */
void
TimeGMST::set_time_by_trunc_julian (
   const double nonsense JEOD_UNUSED)

{
   MessageHandler::fail (
      __FILE__, __LINE__, TimeMessages::invalid_data_error, "\n"
      "Attempted to set GMST time by its Truncated Julian Time value, \n"
      "but GMST has no such valid number.");
   return;
}



/**
 * Destroy a Time_GMST
 */
TimeGMST::~TimeGMST (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
