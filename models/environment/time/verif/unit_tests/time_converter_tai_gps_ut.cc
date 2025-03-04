/*
 * time_converter_tai_gps_ut.cc
 */

#include "environment/time/include/time_converter_tai_gps.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_TAI_GPS, create)
{
    TimeConverter_TAI_GPS staticInst;
    TimeConverter_TAI_GPS * dynInst = new TimeConverter_TAI_GPS;
    delete dynInst;
}

TEST(TimeConverter_TAI_GPS, initialize) {}

TEST(TimeConverter_TAI_GPS, convert_a_to_b) {}

TEST(TimeConverter_TAI_GPS, convert_b_to_a) {}
