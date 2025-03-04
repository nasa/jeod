/*
 * contact_surface_ut.cc
 */

#include "interactions/contact/include/contact_surface.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(ContactSurface, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    ContactSurface staticInst;
    ContactSurface * dynInst = new ContactSurface;
    delete dynInst;
}

TEST(ContactSurface, allocate_array) {}

TEST(ContactSurface, allocate_interaction_facet) {}

TEST(ContactSurface, collect_forces_torques) {}
