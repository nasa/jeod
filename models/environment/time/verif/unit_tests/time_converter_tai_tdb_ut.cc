/*
 * time_converter_tai_tdb_ut.cc
 */

#include "environment/time/include/time_converter_tai_tdb.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeConverter_TAI_TDB, create)
{
    TimeConverter_TAI_TDB staticInst;
    TimeConverter_TAI_TDB * dynInst = new TimeConverter_TAI_TDB;
    delete dynInst;
}

TEST(TimeConverter_TAI_TDB, initialize) {}

TEST(TimeConverter_TAI_TDB, set_a_to_b_offset) {}

TEST(TimeConverter_TAI_TDB, convert_a_to_b) {}

TEST(TimeConverter_TAI_TDB, convert_b_to_a) {}
