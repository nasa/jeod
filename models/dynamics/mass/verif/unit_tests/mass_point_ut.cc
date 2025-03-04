/*
 * mass_point_ut.cc
 */

#include "dynamics/mass/include/mass_point.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(MassPoint, create)
{
    MassPoint staticInst;
    MassPoint * dynInst = new MassPoint;
    delete dynInst;
}

TEST(MassPoint, initialize_mass_point) {}

TEST(MassPoint, compute_relative_state) {}

TEST(MassPoint, compute_state_wrt_pred) {}

TEST(MassPoint, compute_pred_rel_state) {}
