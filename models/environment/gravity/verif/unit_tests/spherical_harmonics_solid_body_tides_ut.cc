/*
 * spherical_harmonics_solid_body_tides_ut.cc
 */

#include "environment/gravity/include/spherical_harmonics_solid_body_tides.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(SphericalHarmonicsSolidBodyTides, create)
{
    SphericalHarmonicsSolidBodyTides staticInst;
    SphericalHarmonicsSolidBodyTides * dynInst = new SphericalHarmonicsSolidBodyTides;
    delete dynInst;
}

TEST(SphericalHarmonicsSolidBodyTides, initialize) {}

TEST(SphericalHarmonicsSolidBodyTides, update) {}
