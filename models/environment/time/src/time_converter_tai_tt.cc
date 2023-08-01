/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_converter_tai_tt.cc
 * Converts between International Atomic Time and Terrestrial Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_converter_tai_tt.cc)
   (time_converter.cc)
   (time.cc)
   (time_tai.cc)
   (time_tt.cc)
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
#include "../include/time_converter_tai_tt.hh"
#include "../include/time_tai.hh"
#include "../include/time_tt.hh"
#include "../include/time_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a TimeConverter_TAI_TT
 */
TimeConverter_TAI_TT::TimeConverter_TAI_TT (
   void)
{
   tai_ptr               = nullptr;
   tt_ptr                = nullptr;
   a_name                = "TAI";
   b_name                = "TT";
   valid_directions = ANY_DIRECTION;
}


/**
 * Initialize the converter.
 * \param[in] parent_ptr Time used to initialize the converter
 * \param[in] child_ptr Other Time used to initialize the converter
 * \param[in] int_dir Conversion direction: +1 a=parent; -1 b=parent; 0 error
 */
void
TimeConverter_TAI_TT::initialize (
   JeodBaseTime * parent_ptr,
   JeodBaseTime * child_ptr,
   const int int_dir)
{
   verify_setup (parent_ptr, child_ptr, int_dir);

   if (int_dir == 1) {
      tai_ptr = dynamic_cast<TimeTAI *> (parent_ptr);
      tt_ptr  = dynamic_cast<TimeTT *> (child_ptr);
   }
   else if (int_dir == -1) {
      tai_ptr = dynamic_cast<TimeTAI *> (child_ptr);
      tt_ptr  = dynamic_cast<TimeTT *> (parent_ptr);
   }
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, TimeMessages::invalid_setup_error, "\n"
         "Illegal value of int_dir in TAI->TT initializer");
   }

   a_to_b_offset = 0.0003725; // 32.184 seconds difference.

   initialized = true;

   return;
}

/**
 * Convert from TimeTAI to TimeTT.
 */
void
TimeConverter_TAI_TT::convert_a_to_b (
   void)
{
   tt_ptr->set_time_by_seconds (tai_ptr->seconds);

   return;
}


/**
 * Convert from TimeTT to TimeTAI.
 *
 * \par Assumptions and Limitations
 *  - Time class MET is based on time class TAI, and counts the elapsed
 *     TAI time only
 */
void
TimeConverter_TAI_TT::convert_b_to_a (
   void)
{
   tai_ptr->set_time_by_seconds (tt_ptr->seconds);

   return;
}


/**
 * Destroy a TimeConverter_TAI_TT
 */
TimeConverter_TAI_TT::~TimeConverter_TAI_TT (
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
