/*
 * aero_drag_ut.cc
 */

#include "interactions/aerodynamics/include/aero_drag.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(AerodynamicDrag, create)
{
    AerodynamicDrag staticInst;
    AerodynamicDrag * dynInst = new AerodynamicDrag;
    delete dynInst;
}

TEST(AerodynamicDrag, aero_drag) {}

TEST(AerodynamicDrag, set_aero_surface) {}

TEST(AerodynamicDrag, clear_aero_surface) {}
