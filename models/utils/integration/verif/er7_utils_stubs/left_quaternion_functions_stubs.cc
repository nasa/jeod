/*
 * left_quaternion_functions_stubs.cc
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#include "er7_utils/integration/core/include/left_quaternion_functions.hh"
#include "utils/sim_interface/include/config.hh"

namespace er7_utils
{

void LeftQuaternionGeneralizedPositionDerivativeFunctions::derivative(
    const double * ER7_UTILS_RESTRICT quat JEOD_UNUSED,
    const double * ER7_UTILS_RESTRICT ang_vel JEOD_UNUSED,
    double * ER7_UTILS_RESTRICT qdot JEOD_UNUSED)
{
}

void LeftQuaternionGeneralizedPositionDerivativeFunctions::second_derivative(
    const double * ER7_UTILS_RESTRICT quat JEOD_UNUSED,
    const double * ER7_UTILS_RESTRICT ang_vel JEOD_UNUSED,
    const double * ER7_UTILS_RESTRICT ang_acc JEOD_UNUSED,
    double * ER7_UTILS_RESTRICT qddot JEOD_UNUSED)
{
}

void LeftQuaternionGeneralizedPositionStepFunctions::dexpinv(const double angular_vel_in[3] JEOD_UNUSED,
                                                             const double dtheta[3] JEOD_UNUSED,
                                                             double angular_vel_out[3] JEOD_UNUSED)
{
}

void LeftQuaternionGeneralizedPositionStepFunctions::expmap(const double quat_init[4] JEOD_UNUSED,
                                                            const double dtheta[3] JEOD_UNUSED,
                                                            double quat_end[4] JEOD_UNUSED)
{
}

} // namespace er7_utils
