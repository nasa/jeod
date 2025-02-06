/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPJ2000
 * @{
 *
 * @file models/environment/RNP/RNPJ2000/src/nutation_j2000_init.cc
 * Implementation of NutationJ2000Init
 */

/*******************************************************************************

Purpose:
  ()

Reference:
      (((Mulcihy, David D. and Bond, Victor R.)
        (The RNP Routine for the STandard Epoch J2000)
        (NASA:JSC-24574) (NASA Engineering Directorate, Navigation, Control,
         and Aeronautics Division) (September 1990) (--)))

Assumptions and limitations:
  ((Earth specific))

Class:
  (NutationJ2000Init)

Library dependencies:
  ((nutation_j2000_init.o)
   (environment/RNP/GenericRNP/planet_rotation_init.o))

 

*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/nutation_j2000_init.hh"


//! Namespace jeod
namespace jeod {

/**
 * constructor. initialize low level data
 */
NutationJ2000Init::NutationJ2000Init (
   void)
:
   num_coeffs(0),
   L_coeffs(NULL),
   M_coeffs(NULL),
   F_coeffs(NULL),
   D_coeffs(NULL),
   omega_coeffs(NULL),
   long_coeffs(NULL),
   long_t_coeffs(NULL),
   obliq_coeffs(NULL),
   obliq_t_coeffs(NULL)
{
// empty for now
}

/**
 * Destructor
 */
NutationJ2000Init::~NutationJ2000Init (
   void)
{
   if (JEOD_IS_ALLOCATED (L_coeffs)) {
      JEOD_DELETE_ARRAY (L_coeffs);
   }
   if (JEOD_IS_ALLOCATED (M_coeffs)) {
      JEOD_DELETE_ARRAY (M_coeffs);
   }
   if (JEOD_IS_ALLOCATED (F_coeffs)) {
      JEOD_DELETE_ARRAY (F_coeffs);
   }
   if (JEOD_IS_ALLOCATED (D_coeffs)) {
      JEOD_DELETE_ARRAY (D_coeffs);
   }
   if (JEOD_IS_ALLOCATED (omega_coeffs)) {
      JEOD_DELETE_ARRAY (omega_coeffs);
   }
   if (JEOD_IS_ALLOCATED (long_coeffs)) {
      JEOD_DELETE_ARRAY (long_coeffs);
   }
   if (JEOD_IS_ALLOCATED (long_t_coeffs)) {
      JEOD_DELETE_ARRAY (long_t_coeffs);
   }
   if (JEOD_IS_ALLOCATED (obliq_coeffs)) {
      JEOD_DELETE_ARRAY (obliq_coeffs);
   }
   if (JEOD_IS_ALLOCATED (obliq_t_coeffs)) {
      JEOD_DELETE_ARRAY (obliq_t_coeffs);
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
