/*
 * mass_point_state_ut.cc
 */

#include "dynamics/mass/include/mass_point_state.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(MassPointState, create)
{
    MassPointState staticInst;
    MassPointState * dynInst = new MassPointState;
    delete dynInst;
}

TEST(MassPointState, initialize_mass_point) {}

TEST(MassPointState, negate) {}

TEST(MassPointState, incr_left) {}

TEST(MassPointState, incr_right) {}

TEST(MassPointState, decr_left) {}

TEST(MassPointState, decr_right) {}
