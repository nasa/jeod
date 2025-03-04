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
  ((surface_model_messages.cc))



*******************************************************************************/

#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/surface_model_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_SURFACEMODEL_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(SurfaceModelMessages, "utils/surface_model/", id)

MAKE_SURFACEMODEL_MESSAGE_CODE(initialization_error);
MAKE_SURFACEMODEL_MESSAGE_CODE(setup_error);
MAKE_SURFACEMODEL_MESSAGE_CODE(runtime_error);

#undef MAKE_SURFACEMODEL_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
