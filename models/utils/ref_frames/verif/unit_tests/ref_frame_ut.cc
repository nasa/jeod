/*
 * ref_frame_ut.cc
 */

#include "utils/ref_frames/include/ref_frame.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RefFrame, create)
{
    RefFrame staticInst;
    RefFrame * dynInst = new RefFrame;
    delete dynInst;
}

TEST(RefFrame, set_active_status) {}

TEST(RefFrame, transplant_node) {}

TEST(RefFrame, reset_parent) {}
