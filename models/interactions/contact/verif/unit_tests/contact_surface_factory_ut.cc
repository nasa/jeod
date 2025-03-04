/*
 * contact_surface_factory_ut.cc
 */

#include "interactions/contact/include/contact_surface_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(ContactSurfaceFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    ContactSurfaceFactory staticInst;
    ContactSurfaceFactory * dynInst = new ContactSurfaceFactory;
    delete dynInst;
}

TEST(ContactSurfaceFactory, create_surface) {}

TEST(ContactSurfaceFactory, add_facet_params) {}
