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
namespace jeod {

// Static member data

char const * IntegrationTestMessages::invalid_data =
    PATH "invalid_data";

char const * IntegrationTestMessages::internal_error =
    PATH "internal_error";

char const * IntegrationTestMessages::debug =
    PATH "debug";

} // End JEOD namespace


#undef PATH
