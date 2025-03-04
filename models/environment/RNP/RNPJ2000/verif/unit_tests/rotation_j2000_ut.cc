/*
 * rotation_j2000_ut.cc
 */

#include "environment/RNP/RNPJ2000/include/rotation_j2000.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RotationJ2000, create)
{
    RotationJ2000 staticInst;
    RotationJ2000 * dynInst = new RotationJ2000;
    delete dynInst;
}

TEST(RotationJ2000, update_rotation) {}
