/*
 * dyn_body_find_body_frame_ut.cc
 */

#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_body/include/dyn_body_messages.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;
using testing::_;

TEST(DynBody, find_body_frame_without_dyn_manager)
{
    MockMessageHandler mockMessageHandler;
    DynBody body;

    EXPECT_CALL(mockMessageHandler,
                process_message(MessageHandler::Failure, _, _, _, DynBodyMessages::invalid_body, _, _))
        .Times(1);

    EXPECT_EQ(nullptr, body.find_body_frame("structure"));
}
