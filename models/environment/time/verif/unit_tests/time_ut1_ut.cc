/*
 * time_ut1_ut.cc
 */

#include "environment/time/include/time_ut1.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeUT1, create)
{
    TimeUT1 staticInst;
    TimeUT1 * dynInst = new TimeUT1;
    delete dynInst;
}

TEST(TimeUT1, set_epoch) {}

TEST(TimeUT1, get_days) {}
