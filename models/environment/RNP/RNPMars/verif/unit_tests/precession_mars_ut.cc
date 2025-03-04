/*
 * precession_mars_ut.cc
 */

#include "environment/RNP/RNPMars/include/precession_mars.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(PrecessionMars, create)
{
    PrecessionMars staticInst;
    PrecessionMars * dynInst = new PrecessionMars;
    delete dynInst;
}

TEST(PrecessionMars, update_rotation) {}

TEST(PrecessionMars, compute_fixed_matrices) {}
