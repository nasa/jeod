/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_point_mass_inertia.cc
 * Compute the inertia tensor of a point mass.
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
  ()


LIBRARY DEPENDENCY:
  ((mass_point_mass_inertia.cc)
   (mass.cc))



******************************************************************************/

// System includes

// Model includes
#include "../include/mass.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Compute the inertia tensor of a point mass.
 * \param[in] mass Mass of point mass\n Units: kg
 * \param[in] r_pt Vector to point mass\n Units: M
 * \param[out] inertia Inertia tensor\n Units: kgM2
 */
void MassBody::compute_point_mass_inertia(double mass, const double r_pt[3], double inertia[3][3])
{
    double rsq[3]; // M2  Squares of components of r

    // Calculate r-squared
    rsq[0] = r_pt[0] * r_pt[0];
    rsq[1] = r_pt[1] * r_pt[1];
    rsq[2] = r_pt[2] * r_pt[2];

    // Compute the moments of inertia.
    inertia[0][0] = mass * (rsq[1] + rsq[2]);
    inertia[1][1] = mass * (rsq[0] + rsq[2]);
    inertia[2][2] = mass * (rsq[0] + rsq[1]);

    // Compute the negative products of inertia.
    inertia[0][1] = -mass * r_pt[0] * r_pt[1];
    inertia[0][2] = -mass * r_pt[0] * r_pt[2];
    inertia[1][2] = -mass * r_pt[1] * r_pt[2];

    // Copy upper triangular elements to the lower triangle.
    inertia[1][0] = inertia[0][1];
    inertia[2][0] = inertia[0][2];
    inertia[2][1] = inertia[1][2];
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
