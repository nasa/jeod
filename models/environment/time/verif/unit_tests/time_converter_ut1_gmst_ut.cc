/*
 * time_converter_ut1_gmst_ut.cc
 */

#include "environment/time/include/time_converter_ut1_gmst.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_UT1_GMST, create)
{
    TimeConverter_UT1_GMST staticInst;
    TimeConverter_UT1_GMST * dynInst = new TimeConverter_UT1_GMST;
    delete dynInst;
}

TEST(TimeConverter_UT1_GMST, initialize) {}

TEST(TimeConverter_UT1_GMST, convert_a_to_b) {}
