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
 * @file models/environment/RNP/RNPJ2000/src/nutation_j2000.cc
 * Implementation for the NutationJ2000 class
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
  ((Earth specific) (Must be initialized))

Class:
  (NutationJ2000)

Library dependencies:
  ((nutation_j2000.cc)
   (nutation_j2000_init.cc)
   (environment/RNP/GenericRNP/src/RNP_messages.cc)
   (environment/RNP/GenericRNP/src/planet_rotation.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/nutation_j2000.hh"
#include "../include/nutation_j2000_init.hh"

//! Namespace jeod
namespace jeod
{

/**
 * destructor
 */
NutationJ2000::~NutationJ2000()
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

/**
 * Specific implementation of update_rotation, from the polymorphic
 * pure virtual base class PlanetRotation
 */
void NutationJ2000::update_rotation()
{
    // This implements the Bond / Vallado implementation of J2000 nutation,
    // as referenced in the documentation
    // compute the fundamental arguments
    // time2 is the square of the time, time3 is the cube
    double time = current_time;
    double time2 = 0.0;
    double time3 = 0.0;

    time2 = time * time;
    time3 = time2 * time;

    // the fundamental arguments are in degrees

    L = 134.9629813888889 + 477198.8673980555 * time + 0.008697222222222223 * time2 + 0.00001777777777777778 * time3;

    M = 357.5277233333333 + 35999.05034 * time - 0.00016027777777777778 * time2 - 0.000003333333333333333 * time3;

    F = 93.27191027777778 + 483202.0175380555 * time - 0.0036825 * time2 + 0.000003055555555555555 * time3;

    D = 297.8503630555556 + 445267.11148 * time - 0.001914166666666667 * time2 + 0.0000052777777777777778 * time3;

    omega = 125.0445222222222 - 1934.136260833333 * time + 0.00207083333333333 * time2 +
            0.000002222222222222222 * time3;

    nutation_in_longitude = 0.0;
    nutation_in_obliquity = 0.0;
    for(unsigned int i = 0; i < num_coeffs; ++i)
    {
        double api = L_coeffs[i] * L + M_coeffs[i] * M + F_coeffs[i] * F + D_coeffs[i] * D + omega_coeffs[i] * omega;
        api *= DEGTORAD;

        nutation_in_longitude += ((long_coeffs[i] + long_t_coeffs[i] * time)) * sin(api);

        nutation_in_obliquity += ((obliq_coeffs[i] + obliq_t_coeffs[i] * time)) * cos(api);

    } // for(unsigned int i = 0)

    // note that the numbers here have been converted from arcseconds to degrees

    epsilon_bar = 23.43929111111111 - 0.01300416666666667 * time - 0.00000016388888889 * time2 +
                  0.00000050361111111 * time3;
    epsilon_bar *= DEGTORAD;

    // based on the JEOD 1.52 code, the nutations in obliquity and
    // longitude are apparently in units of x 10^-4 arcseconds
    // thus these conversions are necessary to get to radians
    // also, to match numerical precesion of JEOD 1.52, some concessions
    // (dividing by DEGTOSEC) are made to the old order of operations
    nutation_in_obliquity = ((nutation_in_obliquity / 10000.0) / 3600.0) * DEGTORAD;

    double epsilon = 0.0;
    epsilon = epsilon_bar + nutation_in_obliquity;

    nutation_in_longitude /= 10000.0;
    double c_eps = cos(epsilon);
    equa_of_equi = (nutation_in_longitude * c_eps) / 15.0;

    nutation_in_longitude = (nutation_in_longitude / 3600) * DEGTORAD;

    // populate the trig functions needed for the rotation matrix
    double c_long = cos(nutation_in_longitude);
    double s_long = sin(nutation_in_longitude);

    double s_eps = sin(epsilon);

    double c_eps_bar = cos(epsilon_bar);
    double s_eps_bar = sin(epsilon_bar);

    Matrix3x3::identity(rotation);

    // populate the rotation matrix
    rotation[0][0] = c_long;
    rotation[0][1] = c_eps * s_long;
    rotation[0][2] = s_eps * s_long;

    rotation[1][0] = -c_eps_bar * s_long;
    rotation[1][1] = c_eps * c_long * c_eps_bar + s_eps * s_eps_bar;
    rotation[1][2] = s_eps * c_long * c_eps_bar - c_eps * s_eps_bar;

    rotation[2][0] = -s_eps_bar * s_long;
    rotation[2][1] = c_eps * c_long * s_eps_bar - s_eps * c_eps_bar;
    rotation[2][2] = s_eps * s_eps_bar * c_long + c_eps * c_eps_bar;
}

/**
 * Initialize the various coefficients needed for the calculation of
 * nutation. init must be of type NutationJ2000Init or a
 * fail message will occur
 * \param[in] init NutationJ2000Init object with needed coefficients
 */
void NutationJ2000::initialize(PlanetRotationInit * init)
{
    // check to see that the init object is of the correct type.
    // cast will return NULL if it is not.
    auto * nut_init = dynamic_cast<NutationJ2000Init *>(init);

    if(nut_init == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RNPMessages::initialization_error,
                             "Init object sent to NutationJ2000 was"
                             " not of type NutationJ2000Init");
        return;
    }

    num_coeffs = nut_init->num_coeffs;

    L_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);
    M_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);
    F_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);
    D_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);
    omega_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);

    long_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);
    long_t_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);
    obliq_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);
    obliq_t_coeffs = JEOD_ALLOC_PRIM_ARRAY(num_coeffs, double);

    for(unsigned int ii = 0; ii < num_coeffs; ++ii)
    {
        L_coeffs[ii] = nut_init->L_coeffs[ii];
        M_coeffs[ii] = nut_init->M_coeffs[ii];
        F_coeffs[ii] = nut_init->F_coeffs[ii];
        D_coeffs[ii] = nut_init->D_coeffs[ii];
        omega_coeffs[ii] = nut_init->omega_coeffs[ii];

        long_coeffs[ii] = nut_init->long_coeffs[ii];
        long_t_coeffs[ii] = nut_init->long_t_coeffs[ii];
        obliq_coeffs[ii] = nut_init->obliq_coeffs[ii];
        obliq_t_coeffs[ii] = nut_init->obliq_t_coeffs[ii];
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
