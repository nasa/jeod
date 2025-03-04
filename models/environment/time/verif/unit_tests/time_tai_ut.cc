/*
 * time_tai_ut.cc
 */

#include "environment/time/include/time_tai.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeTAI, create)
{
    TimeTAI staticInst;
    TimeTAI * dynInst = new TimeTAI;
    delete dynInst;
}

TEST(TimeTAI, set_epoch) {}
