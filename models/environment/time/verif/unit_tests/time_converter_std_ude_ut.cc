/*
 * time_converter_std_ude_ut.cc
 */

#include "environment/time/include/time_converter_std_ude.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_STD_UDE, create)
{
    TimeConverter_STD_UDE staticInst;
    TimeConverter_STD_UDE * dynInst = new TimeConverter_STD_UDE;
    delete dynInst;
}

TEST(TimeConverter_STD_UDE, initialize) {}

TEST(TimeConverter_STD_UDE, convert_a_to_b) {}

TEST(TimeConverter_STD_UDE, convert_b_to_a) {}

TEST(TimeConverter_STD_UDE, reset_a_to_b_offset) {}
