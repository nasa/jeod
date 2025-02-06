/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup OrbitalElements
 * @{
 *
 * @file models/utils/orbital_elements/src/orbital_elements_messages.cc
 * Implement the class OrbitalElementsMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((orbital_elements_messages.cc))

 

*******************************************************************************/


// System includes

// JEOD includes
#include "../include/orbital_elements_messages.hh"

#define PATH "utils/orbital_elements/"


//! Namespace jeod
namespace jeod {

// Static member data

char const * OrbitalElementsMessages::domain_error =
   PATH "domain_error";
char const * OrbitalElementsMessages::convergence_error =
   PATH "convergence_error";

 } // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
