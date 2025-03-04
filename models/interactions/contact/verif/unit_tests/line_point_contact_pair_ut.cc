/*
 * line_point_contact_pair_ut.cc
 */

#include "interactions/contact/include/line_point_contact_pair.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(LinePointContactPair, create)
{
    LinePointContactPair staticInst;
    LinePointContactPair * dynInst = new LinePointContactPair;
    delete dynInst;
}

TEST(LinePointContactPair, in_contact) {}

TEST(LinePointContactPair, initialize_pair) {}
