/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup ModelTemplate
 * @{
 *
 * @file models/utils/model_template/src/template_messages.cc
 * Implement the class TemplateMessages.
 *
 * And in this case, demonstate how to implement message code classes.
 *
 * The MessageHandler defines several methods for informing the user of
 * special conditions that can arise, ranging in severity from fatal errors to
 * detailed debug information. Each of these methods takes a message_code
 * argument. The intent of the message code is to identify the model that
 * invoked the message handler and to characterize the nature of the message.
 *
 * JEOD models implement that intent by making each of the message codes be a
 * static data member of a model-specific message class. Each such data member
 * points to a literal character string of the form "model/path/message_id",
 * where "model/path" is the path to the model, and "message_id" is the
 * stringified data member name.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((template_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/template_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 The goal in a model message code implementation file is to create storage for
 and initialize each of static data members declared in the model message class.
 There are several ways to do this. The examples below target the fictitious
 message class FooMessages defined in the model utils/foo. The class defines
 two message codes, FooMessages::fatal and FooMessages::warning.
 - Most primitive:@verbatim
   const char * FooMessages::fatal = "utils/foo/fatal";
   const char * FooMessages::warning = "utils/foo/warning";
   @endverbatim
 - Recognizing that "utils/foo/" is common:@verbatim
   #define PATH "utils/foo/"
   const char * FooMessages::fatal = PATH "fatal";
   const char * FooMessages::warning = PATH "warning";
   @endverbatim
 - Using the JEOD_MAKE_MESSAGE_CODE macro:@verbatim
   #define PATH "utils/foo/"
   JEOD_MAKE_MESSAGE_CODE (FooMessages, PATH, fatal);
   JEOD_MAKE_MESSAGE_CODE (FooMessages, PATH, warning);
   @endverbatim
 - Using the model-specific macro MAKE_FOO_MESSAGE_CODE:@verbatim
   #define MAKE_FOO_MESSAGE_CODE(id) \
      JEOD_MAKE_MESSAGE_CODE(FooMessages, "utils/foo/", id)
   MAKE_FOO_MESSAGE_CODE (fatal);
   MAKE_FOO_MESSAGE_CODE (warning);
   @endverbatim
*/

/**
 * \def MAKE_TEMPLATE_MESSAGE_CODE(id)
 *   Define TemplateMessages data member @a id.
 * \param id       The name of the data member.
 */
#define MAKE_TEMPLATE_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(TemplateMessages, "utils/model_template/", id)

// Define TemplateMessages static member data

MAKE_TEMPLATE_MESSAGE_CODE(big_negnum);
MAKE_TEMPLATE_MESSAGE_CODE(zero);
MAKE_TEMPLATE_MESSAGE_CODE(big_posnum);

#undef MAKE_TEMPLATE_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
