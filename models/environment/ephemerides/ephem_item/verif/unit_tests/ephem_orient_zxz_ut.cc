/*
 * ephem_orient_zxz_ut.cc
 */

#include "environment/ephemerides/ephem_item/include/ephem_orient_zxz.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(EphemerisZXZOrientation, create)
{
    EphemerisZXZOrientation staticInst;
    EphemerisZXZOrientation * dynInst = new EphemerisZXZOrientation;
    delete dynInst;
}

TEST(EphemerisZXZOrientation, get_euler_angles) {}

TEST(EphemerisZXZOrientation, get_euler_rates) {}

TEST(EphemerisZXZOrientation, propagate) {}

TEST(EphemerisZXZOrientation, update) {}
