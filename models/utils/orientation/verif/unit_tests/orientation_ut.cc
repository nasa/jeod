/*
 * orientation_ut.cc
 */

#include "utils/orientation/include/orientation.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(Orientation, create)
{
    Orientation staticInst;
    Orientation * dynInst = new Orientation;
    delete dynInst;
}

TEST(Orientation, reset) {}

TEST(Orientation, mark_input_as_available) {}

TEST(Orientation, compute_transform) {}

TEST(Orientation, compute_quaternion) {}

TEST(Orientation, compute_eigen_rotation) {}

TEST(Orientation, compute_euler_angles) {}

TEST(Orientation, compute_all_products) {}

TEST(Orientation, compute_transformation_and_quaternion) {}

TEST(Orientation, get_transform) {}

TEST(Orientation, get_quaternion) {}

TEST(Orientation, get_eigen_rotation) {}

TEST(Orientation, get_euler_angles) {}

TEST(Orientation, get_euler_sequence) {}

TEST(Orientation, set_transform) {}

TEST(Orientation, set_quaternion) {}

TEST(Orientation, set_eigen_rotation) {}

TEST(Orientation, set_euler_angles) {}

TEST(Orientation, set_euler_sequence) {}

TEST(Orientation, clear_euler_sequence) {}
