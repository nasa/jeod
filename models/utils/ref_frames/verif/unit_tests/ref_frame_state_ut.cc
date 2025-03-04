/*
 * ref_frame_state_ut.cc
 */

#include "utils/ref_frames/include/ref_frame_state.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RefFrameState, create)
{
    RefFrameState staticInst;
    RefFrameState * dynInst = new RefFrameState;
    delete dynInst;
}

TEST(RefFrameState, negate) {}

TEST(RefFrameState, incr_left) {}

TEST(RefFrameState, incr_right) {}

TEST(RefFrameState, decr_left) {}

TEST(RefFrameState, decr_right) {}
