/*
 * polar_motion_j2000_ut.cc
 */

#include "environment/RNP/RNPJ2000/include/polar_motion_j2000.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(PolarMotionJ2000, create)
{
    PolarMotionJ2000 staticInst;
    PolarMotionJ2000 * dynInst = new PolarMotionJ2000;
    delete dynInst;
}

TEST(PolarMotionJ2000, update_rotation) {}

TEST(PolarMotionJ2000, initialize) {}
