/*
 * time_dyn_ut.cc
 */

#include "environment/time/include/time_dyn.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeDyn, create)
{
    TimeDyn staticInst;
    TimeDyn * dynInst = new TimeDyn;
    delete dynInst;
}

TEST(TimeDyn, initialize_initializer_time) {}

TEST(TimeDyn, update) {}

TEST(TimeDyn, update_offset) {}
