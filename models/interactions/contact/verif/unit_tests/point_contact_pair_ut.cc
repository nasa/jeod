/*
 * point_contact_pair_ut.cc
 */

#include "interactions/contact/include/point_contact_pair.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(PointContactPair, create)
{
    PointContactPair staticInst;
    PointContactPair * dynInst = new PointContactPair;
    delete dynInst;
}

TEST(PointContactPair, in_contact) {}

TEST(PointContactPair, initialize_pair) {}
