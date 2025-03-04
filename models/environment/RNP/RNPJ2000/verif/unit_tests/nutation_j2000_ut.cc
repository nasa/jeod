/*
 * nutation_j2000_ut.cc
 */

#include "environment/RNP/RNPJ2000/include/nutation_j2000.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(NutationJ2000, create)
{
    NutationJ2000 staticInst;
    NutationJ2000 * dynInst = new NutationJ2000;
    delete dynInst;
}

TEST(NutationJ2000, update_rotation) {}

TEST(NutationJ2000, initialize) {}
