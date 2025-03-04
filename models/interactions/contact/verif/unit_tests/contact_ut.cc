/*
 * contact_ut.cc
 */

#include "interactions/contact/include/contact.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(Contact, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Contact staticInst;
    Contact * dynInst = new Contact;
    delete dynInst;
}

TEST(Contact, check_contact) {}

TEST(Contact, initialize_contact) {}

TEST(Contact, unique_pair) {}

TEST(Contact, register_contact) {}

TEST(Contact, register_interaction) {}

TEST(Contact, find_interaction) {}
