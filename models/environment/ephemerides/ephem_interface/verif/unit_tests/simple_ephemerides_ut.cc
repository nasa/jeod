/*
 * simple_ephemerides_ut.cc
 */

#include "environment/ephemerides/ephem_interface/include/simple_ephemerides.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(SinglePointEphemeris, activate) {}

TEST(SinglePointEphemeris, deactivate) {}

TEST(SinglePointEphemeris, set_name) {}

TEST(EmptySpaceEphemeris, create)
{
    EmptySpaceEphemeris staticInst;
    EmptySpaceEphemeris * dynInst = new EmptySpaceEphemeris;
    delete dynInst;
}

TEST(EmptySpaceEphemeris, set_name) {}

TEST(EmptySpaceEphemeris, initialize_model) {}

TEST(EmptySpaceEphemeris, ephem_initialize) {}

TEST(EmptySpaceEphemeris, ephem_activate) {}

TEST(EmptySpaceEphemeris, ephem_build_tree) {}

TEST(SinglePlanetEphemeris, create)
{
    SinglePlanetEphemeris staticInst;
    SinglePlanetEphemeris * dynInst = new SinglePlanetEphemeris;
    delete dynInst;
}

TEST(SinglePlanetEphemeris, set_name) {}

TEST(SinglePlanetEphemeris, initialize_model) {}

TEST(SinglePlanetEphemeris, ephem_initialize) {}

TEST(SinglePlanetEphemeris, ephem_activate) {}

TEST(SinglePlanetEphemeris, ephem_build_tree) {}
