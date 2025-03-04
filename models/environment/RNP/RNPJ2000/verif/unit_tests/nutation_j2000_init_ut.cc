/*
 * nutation_j2000_init_ut.cc
 */

#include "environment/RNP/RNPJ2000/include/nutation_j2000_init.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(NutationJ2000Init, create)
{
    NutationJ2000Init staticInst;
    NutationJ2000Init * dynInst = new NutationJ2000Init;
    delete dynInst;
}
