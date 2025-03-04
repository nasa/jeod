/*
 * radiation_third_body_ut.cc
 */

#include "interactions/radiation_pressure/include/radiation_third_body.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RadiationThirdBody, create)
{
    RadiationThirdBody staticInst;
    RadiationThirdBody * dynInst = new RadiationThirdBody;
    delete dynInst;
}

TEST(RadiationThirdBody, initialize) {}

TEST(RadiationThirdBody, calculate_shadow) {}

TEST(RadiationThirdBody, generate_alpha) {}

TEST(RadiationThirdBody, convert_shadow_from_int) {}

TEST(RadiationThirdBody, process_third_body) {}

TEST(RadiationThirdBody, test_for_state_update) {}

TEST(RadiationThirdBody, update_third_body_state) {}
