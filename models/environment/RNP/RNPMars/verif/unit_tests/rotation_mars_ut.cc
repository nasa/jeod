/*
 * rotation_mars_ut.cc
 */

#include "environment/RNP/RNPMars/include/rotation_mars.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RotationMars, create)
{
    RotationMars staticInst;
    RotationMars * dynInst = new RotationMars;
    delete dynInst;
}

TEST(RotationMars, update_rotation) {}
