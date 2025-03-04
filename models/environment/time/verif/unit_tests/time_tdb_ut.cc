/*
 * time_tdb_ut.cc
 */

#include "environment/time/include/time_tdb.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeTDB, create)
{
    TimeTDB staticInst;
    TimeTDB * dynInst = new TimeTDB;
    delete dynInst;
}

TEST(TimeTDB, set_epoch) {}
