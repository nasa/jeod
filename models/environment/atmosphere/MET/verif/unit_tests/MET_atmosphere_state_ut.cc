/*
 * MET_atmosphere_state_ut.cc
 */

#include "environment/atmosphere/MET/include/MET_atmosphere_state.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(METAtmosphereState, create)
{
    METAtmosphereState staticInst;
    METAtmosphereState * dynInst = new METAtmosphereState;
    delete dynInst;
}

TEST(METAtmosphereState, update_state) {}
