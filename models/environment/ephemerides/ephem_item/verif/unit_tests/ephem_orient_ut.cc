/*
 * ephem_orient_ut.cc
 */

#include "environment/ephemerides/ephem_item/include/ephem_orient.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(EphemerisOrientation, create)
{
    EphemerisOrientation staticInst;
    EphemerisOrientation * dynInst = new EphemerisOrientation;
    delete dynInst;
}

TEST(EphemerisOrientation, updates_what) {}

TEST(EphemerisOrientation, enable) {}

TEST(EphemerisOrientation, note_frame_status_change) {}

TEST(EphemerisOrientation, default_suffix) {}

TEST(EphemerisOrientation, disconnect_from_tree) {}
