/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_std_ude.cc
 * Define member functions for class TimeConverter_STD_UDE.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter_std_ude.cc)
   (time_converter.cc)
   (time.cc)
   (time_standard.cc)
   (time_ude.cc)
   (time_messages.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/message/src/message_handler.cc))

 
******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_converter_std_ude.hh"
#include "../include/time_standard.hh"
#include "../include/time_ude.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_STD_UDE
 */
TimeConverter_STD_UDE::TimeConverter_STD_UDE (
   void)
{
   std_ptr               = nullptr;
   ude_ptr               = nullptr;
   failed_null_test      = false;
   a_name                = "";
   b_name                = "";
   valid_directions = ANY_DIRECTION;
}


/**
 * Initialize the converter.
 *
 * \par Assumptions and Limitations
 *  - This class converts from TimeDyn to TimeUDE *only*
 * \param[in] parent_ptr Time used to initialize the converter
 * \param[in] child_ptr Other Time used to initialize the converter
 * \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 */
void
TimeConverter_STD_UDE::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{
   verify_setup (parent_ptr, child_ptr, int_dir);
   verify_setup (child_ptr, parent_ptr, int_dir);

   if (int_dir == 1) {
      std_ptr = dynamic_cast<TimeStandard *> (parent_ptr);
      ude_ptr = dynamic_cast<TimeUDE *> (child_ptr);
   }
   else if (int_dir == -1) {
      std_ptr = dynamic_cast<TimeStandard *> (child_ptr);
      ude_ptr = dynamic_cast<TimeUDE *> (parent_ptr);
   }

   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Illegal value of int_dir in STD->UDE initializer");
      return;
   }

   if ((std_ptr == nullptr) && (ude_ptr == nullptr) && !failed_null_test) {
      MessageHandler::warn (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Failed to initialize both sides of the converter between %s and %s.\n"
         "Attempting the initialization by switching the value on int_dir,\n"
         "thereby switching the roles of parent and child.\n",
         parent_ptr->name.c_str(), child_ptr->name.c_str());

      initialize (parent_ptr, child_ptr, (int_dir * -1));
      failed_null_test = true;
      return;
   }

   if (std_ptr == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Failed to initialize the STD side of the converter between %s and %s.\n"
         "Neither type is a standard time-type.\n",
         parent_ptr->name.c_str(), child_ptr->name.c_str());
      return;
   }

   if (ude_ptr == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Failed to initialize the UDE side of the converter between %s and %s.\n"
         "Neither type is a UDE time-type.\n",
         parent_ptr->name.c_str(), child_ptr->name.c_str());
      return;
   }

   a_to_b_offset    = ude_ptr->seconds - std_ptr->seconds;
   initialized      = true;
   failed_null_test = false;

   return;
}


/**
 * Convert from TimeSTD to TimeUDE.
 *
 * \par Assumptions and Limitations
 *  - Time class UDE is based on time class STD, and counts the elapsed
 *     STD time only
 */
void
TimeConverter_STD_UDE::convert_a_to_b (
   void)
{
   ude_ptr->set_time_by_seconds (std_ptr->seconds  + a_to_b_offset);

   return;
}


/**
 * Convert from TimeUDE to TimeSTD.
 *
 * \par Assumptions and Limitations
 *  - Time class UDE is based on time class STD, and counts the elapsed
 *     STD time only
 */
void
TimeConverter_STD_UDE::convert_b_to_a (
   void)
{
   std_ptr->set_time_by_seconds (ude_ptr->seconds  - a_to_b_offset);

   return;
}


/**
 * Resets the value of a_to_b_offset.
 */
void
TimeConverter_STD_UDE::reset_a_to_b_offset (
   void)
{
   a_to_b_offset = ude_ptr->seconds - std_ptr->seconds;
   return;
}




/**
 * Destroy a TimeConverter_STD_UDE
 */
TimeConverter_STD_UDE::~TimeConverter_STD_UDE (
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
