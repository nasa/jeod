/*
 * time_gps_ut.cc
 */

#include "environment/time/include/time_gps.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeGPS, create)
{
    TimeGPS staticInst;
    TimeGPS * dynInst = new TimeGPS;
    delete dynInst;
}

TEST(TimeGPS, set_epoch) {}

TEST(TimeGPS, convert_from_calendar) {}

TEST(TimeGPS, calculate_calendar_values) {}

TEST(TimeGPS, set_time_by_seconds) {}

TEST(TimeGPS, set_time_by_days) {}

TEST(TimeGPS, set_time_by_trunc_julian) {}
