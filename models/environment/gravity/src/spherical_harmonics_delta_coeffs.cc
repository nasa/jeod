/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_delta_coeffs.cc
 * Define member functions for the SphericalHarmonicsDeltaCoeffs class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((spherical_harmonics_delta_coeffs.cc)
   (spherical_harmonics_gravity_source.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/spherical_harmonics_delta_coeffs.hh"
#include "../include/spherical_harmonics_delta_coeffs_init.hh"
#include "../include/spherical_harmonics_delta_controls.hh"
#include "../include/spherical_harmonics_gravity_source.hh"

//! Namespace jeod
namespace jeod
{

/**
 * SphericalHarmonicsDeltaCoeffs destructor.
 */
SphericalHarmonicsDeltaCoeffs::~SphericalHarmonicsDeltaCoeffs()
{
    JEOD_DELETE_2D(delta_Cnm, degree + 1, true);
    JEOD_DELETE_2D(delta_Snm, degree + 1, true);
}

/**
 * Initialize the class.
 * \param[in] var_init Init structure
 * \param[in,out] dyn_manager Dynamics manager
 */
void SphericalHarmonicsDeltaCoeffs::initialize(SphericalHarmonicsDeltaCoeffsInit & var_init,
                                               BaseDynManager & dyn_manager JEOD_UNUSED)
{
    // Set degree and order from init structure
    this->degree = var_init.degree;
    this->order = var_init.order;

    // Allocate and load up delta_Cnm and delta_Snm
    if(degree > 0)
    {
        delta_Cnm = JEOD_ALLOC_PRIM_ARRAY(degree + 1, double *);
        delta_Snm = JEOD_ALLOC_PRIM_ARRAY(degree + 1, double *);

        for(unsigned int ii = 0; ii <= degree; ++ii)
        {
            delta_Cnm[ii] = JEOD_ALLOC_PRIM_ARRAY(ii + 1, double);
            delta_Snm[ii] = JEOD_ALLOC_PRIM_ARRAY(ii + 1, double);
        }

        for(unsigned int ii = 0; ii < degree; ++ii)
        {
            for(unsigned int jj = 0; jj < order; ++jj)
            {
                delta_Cnm[ii][jj] = var_init.delta_Cnm[ii][jj];
                delta_Snm[ii][jj] = var_init.delta_Snm[ii][jj];
            }
        }
    }
}

/**
 * Pure virtual update method.
 * \param[in] controls Ignored
 */
void SphericalHarmonicsDeltaCoeffs::update(SphericalHarmonicsGravityControls & controls JEOD_UNUSED)
{
    return; // Pure virtual; no unique behavior of its own
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
