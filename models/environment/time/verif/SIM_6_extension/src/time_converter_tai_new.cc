/******************************************************************************
PURPOSE:
  (Define member functions for class TimeConverter_TAI_New.)

REFERENCE:
  (None)

ASSUMPTIONS AND LIMITATIONS:
  (None)

LIBRARY DEPENDENCY:
  ((time_converter_tai_new.o)
   (time_new.o))

 
******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/time_converter_tai_new.hh"
#include "environment/time/include/time_tai.hh"
#include "../include/time_new.hh"
#include "environment/time/include/time_messages.hh"



//! Namespace jeod
namespace jeod {

/******************************************************************************
 Function: TimeConverter_TAI_New::TimeConverter_TAI_New
 Purpose: (Construct a TimeConverter_TAI_New)
 Class:   (N/A)
******************************************************************************/
TimeConverter_TAI_New::TimeConverter_TAI_New()
{
   tai_ptr = NULL;
   new_ptr = NULL;
   a_name = "TAI";
   b_name = "New";
   valid_directions = A_TO_B;
}


/******************************************************************************
 Function: TimeConverter_TAI_New::initialize
 Purpose: (Initialize the converter.)
 Assumptions: (None)
 Class: (N/A)
******************************************************************************/
void
TimeConverter_TAI_New::initialize(  /* Return: -- Void */
   JeodBaseTime * parent_ptr,       /* In:     -- Time used to initialize the
                                                  converter */
   JeodBaseTime * child_ptr,        /* In:     -- Other Time used to initialize
                                                  the converter */
   const int int_dir)               /* In:     -- Conversion direction:
                                                  +1 a=parent; -1 b=parent; 0 error */
{
  verify_setup(parent_ptr,child_ptr,int_dir);

  if (int_dir == 1) {
    tai_ptr = dynamic_cast<TimeTAI *> (parent_ptr);
    new_ptr = dynamic_cast<TimeNew *> (child_ptr);
  }
  else if (int_dir == -1) {
    tai_ptr = dynamic_cast<TimeTAI *> (child_ptr);
    new_ptr = dynamic_cast<TimeNew *> (parent_ptr);
  }
  else {
    MessageHandler::fail (
      __FILE__, __LINE__, TimeMessages::invalid_setup_error,"\n"
      "Illegal value of int_dir in TAI->New initializer");
  }

  a_to_b_offset = 0.0;

  initialized = true;

  return;
}

/******************************************************************************
 Function: TimeConverter_TAI_New::convert_a_to_b
 Purpose: (Convert from TimeTAI to TimeNew.)
 Assumptions: (Time class MET is based on time class TAI, and counts the elapsed
               TAI time only)
 Class: (N/A)
******************************************************************************/
void
TimeConverter_TAI_New::convert_a_to_b()    // Return:  -- Void
{
  new_ptr->trunc_julian_time = tai_ptr->trunc_julian_time + a_to_b_offset;

  return;
}


/******************************************************************************
 Function: TimeConverter_TAI_New::~TimeConverter_TAI_New
 Purpose: (Destroy a TimeConverter_TAI_New)
 Class:   (N/A)
******************************************************************************/
TimeConverter_TAI_New::~TimeConverter_TAI_New()
{
}
} // End JEOD namespace
