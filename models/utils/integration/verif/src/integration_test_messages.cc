/*******************************************************************************

Purpose:
  (Implement the class IntegrationTestMessages.)

Library dependencies:
  ((integration_test_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/integration_test_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_INTEGRATIONTEST_MESSAGE_CODE(id)                                                                          \
    JEOD_MAKE_MESSAGE_CODE(IntegrationTestMessages, "utils/integration/verif/", id)

// Static member data
MAKE_INTEGRATIONTEST_MESSAGE_CODE(invalid_data);
MAKE_INTEGRATIONTEST_MESSAGE_CODE(internal_error);
MAKE_INTEGRATIONTEST_MESSAGE_CODE(debug);

#undef MAKE_INTEGRATIONTEST_MESSAGE_CODE

} // namespace jeod

#undef PATH
