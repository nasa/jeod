/*
 * alt_lat_long_state_ut.cc
 */

#include "utils/planet_fixed/planet_fixed_posn/include/alt_lat_long_state.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(AltLatLongState, create)
{
    AltLatLongState staticInst;
    AltLatLongState * dynInst = new AltLatLongState;
    delete dynInst;
}

TEST(AltLatLongState, set_data)
{
    AltLatLongState staticInst;
    double alt = 1;
    double lat = 2;
    double lon = 3;
    staticInst.set_data(alt, lat, lon);
    EXPECT_EQ(staticInst.altitude, alt);
    EXPECT_EQ(staticInst.latitude, lat);
    EXPECT_EQ(staticInst.longitude, lon);
}

TEST(AltLatLongState, get_data)
{
    AltLatLongState staticInst;
    staticInst.altitude = 3;
    staticInst.latitude = 2;
    staticInst.longitude = 1;
    double alt;
    double lat;
    double lon;
    staticInst.get_data(alt, lat, lon);
    EXPECT_EQ(staticInst.altitude, alt);
    EXPECT_EQ(staticInst.latitude, lat);
    EXPECT_EQ(staticInst.longitude, lon);
}
