/*
 * contact_params_ut.cc
 */

#include "interactions/contact/include/contact_params.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(ContactParams, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    ContactParams staticInst;
    ContactParams * dynInst = new ContactParams;
    delete dynInst;
}
