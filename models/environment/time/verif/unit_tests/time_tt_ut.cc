/*
 * time_tt_ut.cc
 */

#include "environment/time/include/time_tt.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeTT, create)
{
    TimeTT staticInst;
    TimeTT * dynInst = new TimeTT;
    delete dynInst;
}

TEST(TimeTT, set_epoch) {}
