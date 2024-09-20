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
 * @file models/environment/RNP/RNPJ2000/src/polar_motion_j2000_init.cc
 * Implementation for PolarMotionJ2000Init
 */

/*******************************************************************************

Purpose:
  ()

Reference:
    (((G.H. Kaplan)
      (U.S. Naval Observatory Vector Astrometry Subroutines)
      (NOVAS FORTRAN vers F2.0) (1 Nov 98)))

Assumptions and limitations:
  ((Implementation of PolarMotion is only limited to the data that can be
    put into the initialization file))

Library dependencies:
  ((polar_motion_j2000_init.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/polar_motion_j2000_init.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Destructor
 */
PolarMotionJ2000Init::~PolarMotionJ2000Init()
{
    JEOD_DELETE_ARRAY(polar_mjd);
    JEOD_DELETE_ARRAY(xp_tbl);
    JEOD_DELETE_ARRAY(yp_tbl);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
