/*
 * time_converter_tai_utc_ut.cc
 */

#include "environment/time/include/time_converter_tai_utc.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_TAI_UTC, create)
{
    TimeConverter_TAI_UTC staticInst;
    TimeConverter_TAI_UTC * dynInst = new TimeConverter_TAI_UTC;
    delete dynInst;
}

TEST(TimeConverter_TAI_UTC, initialize) {}

TEST(TimeConverter_TAI_UTC, initialize_leap_second) {}

TEST(TimeConverter_TAI_UTC, convert_a_to_b) {}

TEST(TimeConverter_TAI_UTC, convert_b_to_a) {}

TEST(TimeConverter_TAI_UTC, verify_table_lookup_ends) {}
