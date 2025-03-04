/*
 * spherical_harmonics_tidal_effects_ut.cc
 */

#include "environment/gravity/include/spherical_harmonics_tidal_effects.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(SphericalHarmonicsTidalEffects, create)
{
    SphericalHarmonicsTidalEffects staticInst;
    SphericalHarmonicsTidalEffects * dynInst = new SphericalHarmonicsTidalEffects;
    delete dynInst;
}

TEST(SphericalHarmonicsTidalEffects, initialize) {}

TEST(SphericalHarmonicsTidalEffects, update) {}
