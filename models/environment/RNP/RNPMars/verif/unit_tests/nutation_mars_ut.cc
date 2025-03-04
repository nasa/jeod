/*
 * nutation_mars_ut.cc
 */

#include "environment/RNP/RNPMars/include/nutation_mars.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(NutationMars, create)
{
    NutationMars staticInst;
    NutationMars * dynInst = new NutationMars;
    delete dynInst;
}

TEST(NutationMars, update_rotation) {}
