/*
 * gravity_torque_ut.cc
 */

#include "interactions/gravity_torque/include/gravity_torque.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(GravityTorque, create)
{
    GravityTorque staticInst;
    GravityTorque * dynInst = new GravityTorque;
    delete dynInst;
}

TEST(GravityTorque, initialize) {}

TEST(GravityTorque, update) {}
