/*******************************************************************************

Purpose:
  (Implement the class IntegrationTestMessages.)

Library dependencies:
  ((integration_test_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/integration_test_messages.hh"

#define PATH "utils/integration/verif/"

//! Namespace jeod
namespace jeod
{

// Static member data

const char * IntegrationTestMessages::invalid_data = PATH "invalid_data";

const char * IntegrationTestMessages::internal_error = PATH "internal_error";

const char * IntegrationTestMessages::debug = PATH "debug";

} // namespace jeod

#undef PATH
