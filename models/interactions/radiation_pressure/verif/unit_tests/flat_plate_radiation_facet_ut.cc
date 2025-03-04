/*
 * flat_plate_radiation_facet_ut.cc
 */

#include "interactions/radiation_pressure/include/flat_plate_radiation_facet.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(FlatPlateRadiationFacet, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    FlatPlateRadiationFacet staticInst;
    FlatPlateRadiationFacet * dynInst = new FlatPlateRadiationFacet;
    delete dynInst;
}

TEST(FlatPlateRadiationFacet, define_facet) {}

TEST(FlatPlateRadiationFacet, initialize_geom) {}

TEST(FlatPlateRadiationFacet, incident_radiation) {}

TEST(FlatPlateRadiationFacet, radiation_pressure) {}
