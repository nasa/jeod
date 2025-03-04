/*
 * time_converter_dyn_tai_ut.cc
 */

#include "environment/time/include/time_converter_dyn_tai.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_Dyn_TAI, create)
{
    TimeConverter_Dyn_TAI staticInst;
    TimeConverter_Dyn_TAI * dynInst = new TimeConverter_Dyn_TAI;
    delete dynInst;
}

TEST(TimeConverter_Dyn_TAI, initialize) {}

TEST(TimeConverter_Dyn_TAI, convert_a_to_b) {}
