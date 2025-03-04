/*
 * MET_atmosphere_state_vars_ut.cc
 */

#include "environment/atmosphere/MET/include/MET_atmosphere_state_vars.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(METAtmosphereStateVars, create)
{
    METAtmosphereStateVars staticInst;
    METAtmosphereStateVars * dynInst = new METAtmosphereStateVars;
    delete dynInst;
}
