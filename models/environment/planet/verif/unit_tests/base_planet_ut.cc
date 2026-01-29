/*
 * base_planet_ut.cc
 */

#include "environment/planet/include/base_planet.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(BasePlanet, create)
{
    BasePlanet staticInst;
    BasePlanet * dynInst = new BasePlanet;
    delete dynInst;
}

TEST(BasePlanet, set_alt_inertial) {}

TEST(BasePlanet, set_alt_pfix) {}

TEST(BasePlanet, calculate_alt_pfix) {}

TEST(BasePlanet, register_planet) {}
