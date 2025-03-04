/*
 * north_east_down_ut.cc
 */

#include "utils/planet_fixed/north_east_down/include/north_east_down.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(NorthEastDown, create)
{
    NorthEastDown staticInst;
    NorthEastDown * dynInst = new NorthEastDown;
    delete dynInst;
}

TEST(NorthEastDown, update_from_cart) {}

TEST(NorthEastDown, update_from_spher) {}

TEST(NorthEastDown, update_from_ellip) {}

TEST(NorthEastDown, build_ned_orientation) {}

TEST(NorthEastDown, set_ned_trans_states) {}
