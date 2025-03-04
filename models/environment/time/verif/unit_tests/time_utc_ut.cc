/*
 * time_utc_ut.cc
 */

#include "environment/time/include/time_utc.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeUTC, create)
{
    TimeUTC staticInst;
    TimeUTC * dynInst = new TimeUTC;
    delete dynInst;
}

TEST(TimeUTC, set_epoch) {}
