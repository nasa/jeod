/*
 * time_met_ut.cc
 */

#include "environment/time/include/time_met.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeMET, create)
{
    TimeMET staticInst;
    TimeMET * dynInst = new TimeMET;
    delete dynInst;
}

TEST(TimeMET, update) {}
