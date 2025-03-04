/*
 * time_converter_tai_ut1_ut.cc
 */

#include "environment/time/include/time_converter_tai_ut1.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_TAI_UT1, create)
{
    TimeConverter_TAI_UT1 staticInst;
    TimeConverter_TAI_UT1 * dynInst = new TimeConverter_TAI_UT1;
    delete dynInst;
}

TEST(TimeConverter_TAI_UT1, initialize) {}

TEST(TimeConverter_TAI_UT1, initialize_tai_to_ut1) {}

TEST(TimeConverter_TAI_UT1, convert_a_to_b) {}

TEST(TimeConverter_TAI_UT1, convert_b_to_a) {}

TEST(TimeConverter_TAI_UT1, verify_table_lookup_ends) {}
