/*
 * quat_ut.cc
 */

#include "utils/quaternion/include/quat.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(Quaternion, create)
{
    Quaternion staticInst;
    Quaternion * dynInst = new Quaternion;
    delete dynInst;
}

TEST(Quaternion, compute_slerp) {}
