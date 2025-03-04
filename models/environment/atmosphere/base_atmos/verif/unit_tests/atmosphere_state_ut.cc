/*
 * atmosphere_state_ut.cc
 */

#include "environment/atmosphere/base_atmos/include/atmosphere_state.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(AtmosphereState, create)
{
    AtmosphereState staticInst;
    AtmosphereState * dynInst = new AtmosphereState;
    delete dynInst;
}

TEST(AtmosphereState, update_state) {}

TEST(AtmosphereState, update_wind) {}
