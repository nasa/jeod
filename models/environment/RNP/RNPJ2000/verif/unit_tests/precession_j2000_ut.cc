/*
 * precession_j2000_ut.cc
 */

#include "environment/RNP/RNPJ2000/include/precession_j2000.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(PrecessionJ2000, create)
{
    PrecessionJ2000 staticInst;
    PrecessionJ2000 * dynInst = new PrecessionJ2000;
    delete dynInst;
}

TEST(PrecessionJ2000, update_rotation) {}
