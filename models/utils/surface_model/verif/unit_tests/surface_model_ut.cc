/*
 * surface_model_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/surface_model/include/surface_model.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(SurfaceModel, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SurfaceModel staticInst;
    SurfaceModel * dynInst = new SurfaceModel;
    delete dynInst;
}

TEST(SurfaceModel, add_facets) {}

TEST(SurfaceModel, add_facet) {}

TEST(SurfaceModel, initialize_mass_connections) {}

TEST(SurfaceModel, update_articulation) {}
