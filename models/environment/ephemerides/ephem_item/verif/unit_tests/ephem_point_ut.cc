/*
 * ephem_point_ut.cc
 */

#include "environment/ephemerides/ephem_item/include/ephem_point.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using namespace jeod;

TEST(EphemerisPoint, create)
{
    EphemerisPoint staticInst;
    EphemerisPoint * dynInst = new EphemerisPoint;
    delete dynInst;
}

TEST(EphemerisPoint, note_frame_status_change) {}

TEST(EphemerisPoint, default_suffix) {}

TEST(EphemerisPoint, disconnect_from_tree) {}

TEST(EphemerisPoint, initialize_state) {}

TEST(EphemerisPoint, update) {}

TEST(EphemerisPoint, update_scaled) {}

TEST(EphemerisPoint, updates_what) {}
