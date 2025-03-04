/*
 * time_ude_ut.cc
 */

#include "environment/time/include/time_ude.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeUDE, create)
{
    TimeUDE staticInst;
    TimeUDE * dynInst = new TimeUDE;
    delete dynInst;
}

TEST(TimeUDE, must_be_singleton) {}

TEST(TimeUDE, add_type_initialize) {}

TEST(TimeUDE, convert_epoch_to_update) {}

TEST(TimeUDE, initialize_from_parent) {}

TEST(TimeUDE, initialize_initializer_time) {}

TEST(TimeUDE, set_epoch_times) {}

TEST(TimeUDE, set_epoch_dyn) {}

TEST(TimeUDE, set_epoch_std) {}

TEST(TimeUDE, set_epoch_ude) {}

TEST(TimeUDE, set_initial_times) {}

TEST(TimeUDE, set_time_by_days) {}

TEST(TimeUDE, set_time_by_seconds) {}

TEST(TimeUDE, set_time_by_clock) {}

TEST(TimeUDE, set_epoch_initializing_value) {}

TEST(TimeUDE, clock_update) {}

TEST(TimeUDE, verify_epoch) {}

TEST(TimeUDE, verify_init) {}

TEST(TimeUDE, verify_update) {}
