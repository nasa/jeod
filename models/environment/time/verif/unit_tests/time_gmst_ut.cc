/*
 * time_gmst_ut.cc
 */

#include "environment/time/include/time_gmst.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeGMST, create)
{
    TimeGMST staticInst;
    TimeGMST * dynInst = new TimeGMST;
    delete dynInst;
}

TEST(TimeGMST, calculate_calendar_values) {}

TEST(TimeGMST, set_time_by_trunc_julian) {}
