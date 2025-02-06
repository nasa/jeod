/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_tai_gps.cc
 * Converts between International Atomic Time and the clock associated with the
 * Global Positioning System.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter_tai_gps.cc)
   (time_converter.cc)
   (time.cc)
   (time_tai.cc)
   (time_gps.cc)
   (time_messages.cc)
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
#include "../include/time_converter_tai_gps.hh"
#include "../include/time_tai.hh"
#include "../include/time_gps.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_TAI_GPS
 */
TimeConverter_TAI_GPS::TimeConverter_TAI_GPS (
   void)
{
   tai_ptr               = nullptr;
   gps_ptr               = nullptr;
   a_name                = "TAI";
   b_name                = "GPS";
   valid_directions = ANY_DIRECTION;
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
TimeConverter_TAI_GPS::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{
   verify_setup (parent_ptr, child_ptr, int_dir);


   if (int_dir == 1) {
      tai_ptr = dynamic_cast<TimeTAI *> (parent_ptr);
      gps_ptr = dynamic_cast<TimeGPS *> (child_ptr);
   }
   else if (int_dir == -1) {
      tai_ptr = dynamic_cast<TimeTAI *> (child_ptr);
      gps_ptr = dynamic_cast<TimeGPS *> (parent_ptr);
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Illegal value of int_dir in TAI->GPS initializer");
   }

   a_to_b_offset = (tai_ptr->tjt_at_epoch - gps_ptr->tjt_at_epoch) * 86400;

   initialized = true;

   return;
}

/**
 * Convert from TimeTAI to TimeGPS.
 */
void
TimeConverter_TAI_GPS::convert_a_to_b (
   void)
{
   gps_ptr->set_time_by_seconds (tai_ptr->seconds  + a_to_b_offset);

   return;
}

/**
 * Convert from TimeGPS to TimeTAI.
 */
void
TimeConverter_TAI_GPS::convert_b_to_a (
   void)
{
   tai_ptr->set_time_by_seconds (gps_ptr->seconds  - a_to_b_offset);

   return;
}


/**
 * Destroy a TimeConverter_TAI_GPS
 */
TimeConverter_TAI_GPS::~TimeConverter_TAI_GPS (
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
