/*
 * rnp_mars_ut.cc
 */

#include "environment/RNP/RNPMars/include/rnp_mars.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RNPMars, create)
{
    RNPMars staticInst;
    RNPMars * dynInst = new RNPMars;
    delete dynInst;
}

TEST(RNPMars, initialize) {}

TEST(RNPMars, update_rnp) {}

TEST(RNPMars, update_axial_rotation) {}

TEST(RNPMars, timestamp) {}

TEST(RNPMars, get_name) {}

TEST(RNPMars, ephem_update) {}

TEST(RNPMars, get_dyn_time_ptr) {}
