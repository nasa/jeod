/*
 * point_contact_facet_ut.cc
 */

#include "interactions/contact/include/point_contact_facet.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(PointContactFacet, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    PointContactFacet staticInst;
    PointContactFacet * dynInst = new PointContactFacet;
    delete dynInst;
}

TEST(PointContactFacet, create_pair) {}

TEST(PointContactFacet, calculate_contact_point) {}

TEST(PointContactFacet, set_max_dimension)
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

TEST(PointContactFacet, calculate_torque) {}
