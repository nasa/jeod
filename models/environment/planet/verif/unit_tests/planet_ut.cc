/*
 * planet_ut.cc
 */

#include "environment/planet/include/planet.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(Planet, create)
{
    Planet staticInst;
    Planet * dynInst = new Planet;
    delete dynInst;
}

TEST(Planet, register_model) {}

TEST(Planet, initialize) {}
