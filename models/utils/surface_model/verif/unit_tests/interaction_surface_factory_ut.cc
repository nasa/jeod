/*
 * interaction_surface_factory_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/surface_model/include/interaction_surface_factory.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(InteractionSurfaceFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    InteractionSurfaceFactory staticInst;
    InteractionSurfaceFactory * dynInst = new InteractionSurfaceFactory;
    delete dynInst;
}

TEST(InteractionSurfaceFactory, create_surface) {}

TEST(InteractionSurfaceFactory, add_facet_factory) {}

TEST(InteractionSurfaceFactory, add_facet_params) {}
