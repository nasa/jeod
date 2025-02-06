/*******************************************************************************

Purpose:
  (Implement the class OrientationMessages.)

Library dependencies:
  ((orientation_messages.cc))

 

*******************************************************************************/


// System includes

// JEOD includes
#include "../include/orientation_messages.hh"

#define PATH "utils/integration/verif/"


//! Namespace jeod
namespace jeod {

// Static member data
char const * OrientationMessages::invalid_enum =
    PATH "invalid_enum";

char const * OrientationMessages::invalid_data =
    PATH "invalid_data";

char const * OrientationMessages::invalid_request =
    PATH "invalid_request";

} // End JEOD namespace

#undef PATH
