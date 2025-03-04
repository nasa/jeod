/*
 * line_contact_facet_factory_ut.cc
 */

#include "interactions/contact/include/line_contact_facet_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(LineContactFacetFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    LineContactFacetFactory staticInst;
    LineContactFacetFactory * dynInst = new LineContactFacetFactory;
    delete dynInst;
}

TEST(LineContactFacetFactory, create_facet) {}

TEST(LineContactFacetFactory, is_correct_factory) {}
