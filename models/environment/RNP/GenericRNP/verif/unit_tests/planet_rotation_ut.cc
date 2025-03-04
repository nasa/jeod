/*
 * planet_rotation_ut.cc
 */

#include "environment/RNP/GenericRNP/include/planet_rotation.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(PlanetRotation, create)
{
    PlanetRotation staticInst;
    PlanetRotation * dynInst = new PlanetRotation;
    delete dynInst;
}

TEST(PlanetRotation, update_time) {}

TEST(PlanetRotation, get_rotation) {}

TEST(PlanetRotation, get_rotation_transpose) {}
