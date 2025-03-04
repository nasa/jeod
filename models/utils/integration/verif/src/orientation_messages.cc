/*******************************************************************************

Purpose:
  (Implement the class OrientationMessages.)

Library dependencies:
  ((orientation_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/orientation_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_ORIENTATION_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(OrientationMessages, "utils/integration/verif/", id)

// Static member data
MAKE_ORIENTATION_MESSAGE_CODE(invalid_enum);
MAKE_ORIENTATION_MESSAGE_CODE(invalid_data);
MAKE_ORIENTATION_MESSAGE_CODE(invalid_request);

#undef MAKE_ORIENTATION_MESSAGE_CODE

} // namespace jeod

#undef PATH
