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
  ((nutation_j2000_init.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/nutation_j2000_init.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Destructor
 */
NutationJ2000Init::~NutationJ2000Init()
{
    JEOD_DELETE_ARRAY(L_coeffs);
    JEOD_DELETE_ARRAY(M_coeffs);
    JEOD_DELETE_ARRAY(F_coeffs);
    JEOD_DELETE_ARRAY(D_coeffs);
    JEOD_DELETE_ARRAY(omega_coeffs);
    JEOD_DELETE_ARRAY(long_coeffs);
    JEOD_DELETE_ARRAY(long_t_coeffs);
    JEOD_DELETE_ARRAY(obliq_coeffs);
    JEOD_DELETE_ARRAY(obliq_t_coeffs);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
