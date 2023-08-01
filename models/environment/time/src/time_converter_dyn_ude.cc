/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_dyn_ude.cc
 * Converts between Dynamic Time and a time with User-Defined-Epoch.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter_dyn_ude.cc)
   (time_converter.cc)
   (time.cc)
   (time_dyn.cc)
   (time_ude.cc)
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
#include "../include/time_converter_dyn_ude.hh"
#include "../include/time_dyn.hh"
#include "../include/time_ude.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_Dyn_UDE
 */
TimeConverter_Dyn_UDE::TimeConverter_Dyn_UDE (
   void)
{
   dyn_ptr               = nullptr;
   ude_ptr               = nullptr;
   a_name                = "Dyn";
   b_name                = "";
   valid_directions = A_TO_B;
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
TimeConverter_Dyn_UDE::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{

   // Direction = 1 => Translate from parent (TimeDyn) to child (TimeUDE)
   // Note: This is the only direction supported by this converter.
   if (int_dir == 1) {
      verify_setup (child_ptr, parent_ptr, int_dir);

      // Convert the parent to a TimeDyn, ensuring that this conversion works.
      dyn_ptr = dynamic_cast<TimeDyn *> (parent_ptr);
      if (dyn_ptr == nullptr) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
            "Object '%s' is not a %s object as expected with int_dir = 1\n",
            parent_ptr->name.c_str(), "TimeDyn");
         return;
      }

      // Convert the parent to a TimeUDE, ensuring that this conversion works.
      ude_ptr = dynamic_cast<TimeUDE *> (child_ptr);
      if (ude_ptr == nullptr) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
            "Object '%s' is not a %s object as expected with int_dir = 1\n",
            child_ptr->name.c_str(), "TimeUDE");
         return;
      }
   }

   // This converter does not accommodate a reverse conversion.
   else if (int_dir == -1) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::incomplete_setup_error, "\n"
         "There is no converter available for converting UDE to Dyn.\n");
      return;
   }

   // All other int_dirs are *always* invalid.
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Illegal value of int_dir in Dyn->UDE initializer");
      return;
   }

   // Compute the initial offset.
   a_to_b_offset = ude_ptr->seconds - dyn_ptr->seconds;

   initialized = true;

   return;
}


/**
 * Convert from TimeDyn to TimeUDE.
 *
 * \par Assumptions and Limitations
 *  - Time class UDE is based on time class TAI, and counts the elapsed
 *             TAI time only
 */
void
TimeConverter_Dyn_UDE::convert_a_to_b (
   void)
{
   ude_ptr->set_time_by_seconds (dyn_ptr->seconds  + a_to_b_offset);
   // ude_ptr->days = ude_ptr->seconds / 86400;

   return;
}


/**
 * Resets the value of a_to_b_offset.
 */
void
TimeConverter_Dyn_UDE::reset_a_to_b_offset (
   void)
{
   a_to_b_offset = ude_ptr->seconds - dyn_ptr->seconds;
   return;
}

/**
 * Destroy a TimeConverter_Dyn_UDE
 */
TimeConverter_Dyn_UDE::~TimeConverter_Dyn_UDE (
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
