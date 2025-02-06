/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_dyn_tai.cc
 * Converts between International Atomic Time and Dynamic Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((This class converts from TimeDyn to TimeTAI *only*))

LIBRARY DEPENDENCY:
  ((time_converter_dyn_tai.o)
   (time_converter.o)
   (time.o)
   (time_dyn.o)
   (time_tai.o)
   (time_messages.o)
   (utils/sim_interface/memory_interface.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o))

 
******************************************************************************/

// System includes
#include <cstddef>
#include <cmath>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

   // Model includes
#include "../include/time_converter_dyn_tai.hh"
#include "../include/time_dyn.hh"
#include "../include/time_tai.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_Dyn_TAI
 */
TimeConverter_Dyn_TAI::TimeConverter_Dyn_TAI (
   void)
{
   dyn_ptr               = NULL;
   tai_ptr               = NULL;
   a_name                = "Dyn";
   b_name                = "TAI";
   valid_directions = A_TO_B_UPDATE;
}


/**
 * Initialize the converter.
 * \param[in] parent_ptr Time used to initialize the converter
 * \param[in] child_ptr Other Time used to initialize the converter
 * \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 */
void
TimeConverter_Dyn_TAI::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{
// NOT const Time *; will be casting to different type.

   // Direction = 1 => Translate from parent (TimeDyn) to child (TimeTAI)
   // Note: This converter will only support conversions from Dyn to TAI, but
   // must be initialized from TAI.  If TAI has not been initialized at this
   // point, the verify_setup routines will fail the simulation.

   if (int_dir == 1) {
      verify_setup (child_ptr, parent_ptr, int_dir);

      // Convert the parent to a TimeDyn, ensuring that this conversion works.
      dyn_ptr = dynamic_cast<TimeDyn *> (parent_ptr);
      if (dyn_ptr == NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error,
            "\n Object '%s' is not an object of type %s, as was expected when"
            "initializing the converter with int_dir = 1. \n"
            "TimeDyn", parent_ptr->name.c_str());
      }

      // Convert the child to a TimeTAI, ensuring that this conversion works.
      tai_ptr = dynamic_cast<TimeTAI *> (child_ptr);
      if (tai_ptr == NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error,
            "\n Object '%s' is not an object of type %s, as was expected when"
            "initializing the converter with int_dir = 1.\n"
            "TimeTAI", child_ptr->name.c_str());
      }
   }


   else if (int_dir == -1) {
      verify_setup (parent_ptr, child_ptr, int_dir);


      // Convert the child to a TimeDyn, ensuring that this conversion works.
      dyn_ptr = dynamic_cast<TimeDyn *> (child_ptr);
      if (dyn_ptr == NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error,
            "\n Object '%s' is not an object of type %s, as was expected when"
            "initializing the converter with int_dir = -1.\n"
            "TimeDyn", child_ptr);
      }

      // Convert the parent to a TimeTAI, ensuring that this conversion works.
      tai_ptr = dynamic_cast<TimeTAI *> (parent_ptr);
      if (tai_ptr == NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, TimeMessages::invalid_setup_error,
            "\n Object '%s' is not an object of type %s, as was expected when"
            "initializing the converter with int_dir = -1.\n",
            "TimeTAI", parent_ptr);
      }
   }

   // All other int_dirs are *always* invalid.
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error,
         "\nIllegal value of int_dir in Dyn->TAI initializer.\n");
   }

   // Compute the initial offset.
   if (std::fpclassify(dyn_ptr->seconds) != FP_ZERO) {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::initialization_error, "\n"
         "DynTime must be 0 for the Dyn-TAI converter to be "
         "initialized.\n");
   }
   a_to_b_offset = tai_ptr->seconds;

   initialized = true;

   return;
}


/**
 * Convert from TimeDyn to TimeTAI.
 */
void
TimeConverter_Dyn_TAI::convert_a_to_b (
   void)
{
   // 86400 seconds in a day

   tai_ptr->set_time_by_seconds (dyn_ptr->seconds + a_to_b_offset);
//  tai_ptr->days = tai_ptr->seconds / 86400;
//  tai_ptr->trunc_julian_time = tai_ptr->days + tai_ptr->tjt_at_epoch;

   return;
}


/**
 * Destroy a TimeConverter_Dyn_TAI
 */
TimeConverter_Dyn_TAI::~TimeConverter_Dyn_TAI (
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
