/*
 * polar_motion_j2000_init_ut.cc
 */

#include "environment/RNP/RNPJ2000/include/polar_motion_j2000_init.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(PolarMotionJ2000Init, create)
{
    PolarMotionJ2000Init staticInst;
    PolarMotionJ2000Init * dynInst = new PolarMotionJ2000Init;
    delete dynInst;
}
