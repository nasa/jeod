/*******************************************************************************

Purpose:
  (Implement the class MessageHandlerVerifMessages.)

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((message_handler_verif_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/message_handler_verif_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_MESSAGEHANDLERVERIF_MESSAGE_CODE(id)                                                                      \
    JEOD_MAKE_MESSAGE_CODE(MessageHandlerVerifMessages, "utils/message/verif/", id)

// Static member data
MAKE_MESSAGEHANDLERVERIF_MESSAGE_CODE(error);
MAKE_MESSAGEHANDLERVERIF_MESSAGE_CODE(warning);
MAKE_MESSAGEHANDLERVERIF_MESSAGE_CODE(inform);
MAKE_MESSAGEHANDLERVERIF_MESSAGE_CODE(debug);
MAKE_MESSAGEHANDLERVERIF_MESSAGE_CODE(message);

#undef MAKE_MESSAGEHANDLERVERIF_MESSAGE_CODE

} // namespace jeod
