/*
 * time_manager_ut.cc
 */

#include "environment/time/include/time_manager.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(TimeManager, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    TimeManager staticInst;
    TimeManager * dynInst = new TimeManager;
    delete dynInst;
}

TEST(TimeManager, get_converter_ptr) {}

TEST(TimeManager, get_jeod_integration_time) {}

TEST(TimeManager, get_time_change_flag) {}

TEST(TimeManager, get_time_scale_factor) {}

TEST(TimeManager, get_timestamp_time) {}

TEST(TimeManager, get_time_ptr) {}

TEST(TimeManager, register_time) {}

TEST(TimeManager, register_time_named) {}

TEST(TimeManager, register_converter) {}

TEST(TimeManager, time_standards_exist) {}

TEST(TimeManager, time_lookup) {}

TEST(TimeManager, update) {}

TEST(TimeManager, update_time) {}

TEST(TimeManager, verify_table_lookup_ends) {}
