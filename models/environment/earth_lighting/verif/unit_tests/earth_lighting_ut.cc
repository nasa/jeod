/*
 * earth_lighting_ut.cc
 */

#include "environment/earth_lighting/include/earth_lighting.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(EarthLighting, create)
{
    EarthLighting staticInst;
    EarthLighting * dynInst = new EarthLighting;
    delete dynInst;
}

TEST(EarthLighting, initialize) {}

TEST(EarthLighting, circle_intersect) {}

TEST(EarthLighting, calc_lighting) {}
