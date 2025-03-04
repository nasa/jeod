/*
 * thermal_integrable_object_ut.cc
 */

#include "interactions/thermal_rider/include/thermal_integrable_object.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(ThermalIntegrableObject, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    ThermalIntegrableObject staticInst;
    ThermalIntegrableObject * dynInst = new ThermalIntegrableObject;
    delete dynInst;
}

TEST(ThermalIntegrableObject, create_integrators) {}

TEST(ThermalIntegrableObject, destroy_integrators) {}

TEST(ThermalIntegrableObject, reset_integrators) {}

TEST(ThermalIntegrableObject, integrate) {}

TEST(ThermalIntegrableObject, initialize) {}

TEST(ThermalIntegrableObject, compute_temp_dot) {}
