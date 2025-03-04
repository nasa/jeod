/*
 * time_converter_dyn_tdb_ut.cc
 */

#include "environment/time/include/time_converter_dyn_tdb.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_Dyn_TDB, create)
{
    TimeConverter_Dyn_TDB staticInst;
    TimeConverter_Dyn_TDB * dynInst = new TimeConverter_Dyn_TDB;
    delete dynInst;
}

TEST(TimeConverter_Dyn_TDB, initialize) {}

TEST(TimeConverter_Dyn_TDB, convert_a_to_b) {}
