/*
 * time_converter_dyn_ude_ut.cc
 */

#include "environment/time/include/time_converter_dyn_ude.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_Dyn_UDE, create)
{
    TimeConverter_Dyn_UDE staticInst;
    TimeConverter_Dyn_UDE * dynInst = new TimeConverter_Dyn_UDE;
    delete dynInst;
}

TEST(TimeConverter_Dyn_UDE, initialize) {}

TEST(TimeConverter_Dyn_UDE, convert_a_to_b) {}

TEST(TimeConverter_Dyn_UDE, reset_a_to_b_offset) {}
