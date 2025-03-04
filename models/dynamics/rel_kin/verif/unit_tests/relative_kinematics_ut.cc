/*
 * relative_kinematics_ut.cc
 */

#include "dynamics/rel_kin/include/relative_kinematics.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(RelativeKinematics, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    RelativeKinematics staticInst;
    RelativeKinematics * dynInst = new RelativeKinematics;
    delete dynInst;
}

TEST(RelativeKinematics, add_relstate) {}

TEST(RelativeKinematics, remove_relstate) {}

TEST(RelativeKinematics, find_relstate) {}

TEST(RelativeKinematics, activate_relstate) {}

TEST(RelativeKinematics, update_single) {}

TEST(RelativeKinematics, update_all) {}
