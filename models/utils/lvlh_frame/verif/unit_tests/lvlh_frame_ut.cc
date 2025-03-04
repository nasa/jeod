/*
 * lvlh_frame_ut.cc
 */

#include "utils/lvlh_frame/include/lvlh_frame.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(LvlhFrame, create)
{
    LvlhFrame staticInst;
    LvlhFrame * dynInst = new LvlhFrame;
    delete dynInst;
}

TEST(LvlhFrame, initialize) {}

TEST(LvlhFrame, update) {}

TEST(LvlhFrame, set_subject_name) {}

TEST(LvlhFrame, set_subject_frame) {}

TEST(LvlhFrame, set_planet_name) {}

TEST(LvlhFrame, set_planet) {}

TEST(LvlhFrame, compute_lvlh_frame) {}
