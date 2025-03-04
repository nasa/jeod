/*
 * thermal_model_rider_ut.cc
 */

#include "interactions/thermal_rider/include/thermal_model_rider.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(ThermalModelRider, create)
{
    ThermalModelRider staticInst;
    ThermalModelRider * dynInst = new ThermalModelRider;
    delete dynInst;
}

TEST(ThermalModelRider, update) {}
