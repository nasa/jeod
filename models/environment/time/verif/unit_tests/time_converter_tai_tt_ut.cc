/*
 * time_converter_tai_tt_ut.cc
 */

#include "environment/time/include/time_converter_tai_tt.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_TAI_TT, create)
{
    TimeConverter_TAI_TT staticInst;
    TimeConverter_TAI_TT * dynInst = new TimeConverter_TAI_TT;
    delete dynInst;
}

TEST(TimeConverter_TAI_TT, initialize) {}

TEST(TimeConverter_TAI_TT, convert_a_to_b) {}

TEST(TimeConverter_TAI_TT, convert_b_to_a) {}
