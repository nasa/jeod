/*
 * default_aero_ut.cc
 */

#include "interactions/aerodynamics/include/default_aero.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(DefaultAero, create)
{
    DefaultAero staticInst;
    DefaultAero * dynInst = new DefaultAero;
    delete dynInst;
}

TEST(DefaultAero, aerodrag_force) {}
