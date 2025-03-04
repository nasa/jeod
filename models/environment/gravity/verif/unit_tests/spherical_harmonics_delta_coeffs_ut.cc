/*
 * spherical_harmonics_delta_coeffs_ut.cc
 */

#include "environment/gravity/include/spherical_harmonics_delta_coeffs.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(SphericalHarmonicsDeltaCoeffs, create)
{
    SphericalHarmonicsDeltaCoeffs staticInst;
    SphericalHarmonicsDeltaCoeffs * dynInst = new SphericalHarmonicsDeltaCoeffs;
    delete dynInst;
}

TEST(SphericalHarmonicsDeltaCoeffs, initialize) {}

TEST(SphericalHarmonicsDeltaCoeffs, update) {}
