/*
 * flat_plate_ut.cc
 */

#include "utils/surface_model/include/flat_plate.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(FlatPlate, create)
{
    FlatPlate staticInst;
    FlatPlate * dynInst = new FlatPlate;
    delete dynInst;
}

TEST(FlatPlate, update_articulation_internal) {}
