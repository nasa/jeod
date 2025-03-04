/*
 * line_contact_facet_ut.cc
 */

#include "interactions/contact/include/line_contact_facet.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(LineContactFacet, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    LineContactFacet staticInst;
    LineContactFacet * dynInst = new LineContactFacet;
    delete dynInst;
}

TEST(LineContactFacet, create_pair) {}

TEST(LineContactFacet, calculate_contact_point) {}

TEST(LineContactFacet, set_max_dimension)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    PointContactFacet staticInst;
    EXPECT_EQ(staticInst.radius, 0);
    EXPECT_EQ(staticInst.max_dimension, 0);
    staticInst.radius = 123;
    staticInst.set_max_dimension();
    EXPECT_EQ(staticInst.max_dimension, 123);
}

TEST(LineContactFacet, calculate_torque) {}
