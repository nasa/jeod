/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/surface_model_messages.cc
 * Implement surface_model_messages
 */

/*******************************************************************************

Purpose:
  ()

Reference:
   (((None)))

Assumptions and limitations:
   ((Outlined in the header file))

Class:
   (None)

Library dependencies:
  ((surface_model_messages.o))

 

*******************************************************************************/

#include "../include/surface_model_messages.hh"

#define PATH "utils/surface_model/"


//! Namespace jeod
namespace jeod {

char const * SurfaceModelMessages::initialization_error =
   PATH "initialization_error";

char const * SurfaceModelMessages::setup_error =
   PATH "setup_error";

char const * SurfaceModelMessages::runtime_error =
   PATH "runtime_error";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
