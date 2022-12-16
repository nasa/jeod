/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup LvlhFrame
 * @{
 *
 * @file models/utils/lvlh_frame/src/lvlh_frame_messages.cc
 * Implement the class LvlhFrameMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((lvlh_frame_messages.o))

 

*******************************************************************************/


// System includes

// JEOD includes
#include "../include/lvlh_frame_messages.hh"

#define PATH "utils/lvlh_frame/"


//! Namespace jeod
namespace jeod {

// Static member data
char const * LvlhFrameMessages::fatal_error =
    PATH "fatal_error";

char const * LvlhFrameMessages::illegal_value =
    PATH "illegal_value";

char const * LvlhFrameMessages::invalid_name =
    PATH "invalid_name";

char const * LvlhFrameMessages::invalid_configuration =
    PATH "invalid_configuration";

char const * LvlhFrameMessages::invalid_object =
    PATH "invalid_object";

char const * LvlhFrameMessages::null_pointer =
    PATH "null_pointer";

char const * LvlhFrameMessages::trace =
    PATH "trace";

char const * LvlhFrameMessages::divide_by_zero =
   PATH "divide_by_zero";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
