/*
 * point_contact_facet_factory_ut.cc
 */

#include "interactions/contact/include/point_contact_facet_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(PointContactFacetFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    PointContactFacetFactory staticInst;
    PointContactFacetFactory * dynInst = new PointContactFacetFactory;
    delete dynInst;
}

TEST(PointContactFacetFactory, create_facet) {}

TEST(PointContactFacetFactory, is_correct_factory) {}
