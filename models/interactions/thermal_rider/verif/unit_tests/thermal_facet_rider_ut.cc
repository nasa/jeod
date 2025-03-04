/*
 * thermal_facet_rider_ut.cc
 */

#include "interactions/thermal_rider/include/thermal_facet_rider.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(ThermalFacetRider, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    ThermalFacetRider staticInst;
    ThermalFacetRider * dynInst = new ThermalFacetRider;
    delete dynInst;
}

TEST(ThermalFacetRider, accumulate_thermal_sources) {}

TEST(ThermalFacetRider, initialize) {}

TEST(ThermalFacetRider, integrate) {}
