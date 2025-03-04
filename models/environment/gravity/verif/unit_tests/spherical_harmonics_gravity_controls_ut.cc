/*
 * spherical_harmonics_gravity_controls_ut.cc
 */

#include "environment/gravity/include/spherical_harmonics_gravity_controls.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(SphericalHarmonicsGravityControls, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SphericalHarmonicsGravityControls staticInst;
    SphericalHarmonicsGravityControls * dynInst = new SphericalHarmonicsGravityControls;
    delete dynInst;
}

TEST(SphericalHarmonicsGravityControls, initialize_control) {}

TEST(SphericalHarmonicsGravityControls, add_deltacontrol) {}

TEST(SphericalHarmonicsGravityControls, get_degree) {}

TEST(SphericalHarmonicsGravityControls, get_order) {}

TEST(SphericalHarmonicsGravityControls, get_degree_order) {}

TEST(SphericalHarmonicsGravityControls, get_grad_degree) {}

TEST(SphericalHarmonicsGravityControls, get_grad_order) {}

TEST(SphericalHarmonicsGravityControls, get_grad_degree_order) {}

TEST(SphericalHarmonicsGravityControls, set_degree) {}

TEST(SphericalHarmonicsGravityControls, set_order) {}

TEST(SphericalHarmonicsGravityControls, set_degree_order) {}

TEST(SphericalHarmonicsGravityControls, set_grad_degree) {}

TEST(SphericalHarmonicsGravityControls, set_grad_order) {}

TEST(SphericalHarmonicsGravityControls, set_grad_degree_order) {}

TEST(SphericalHarmonicsGravityControls, check_validity) {}

TEST(SphericalHarmonicsGravityControls, update_deltacoeffs) {}

TEST(SphericalHarmonicsGravityControls, sum_deltacoeffs) {}
