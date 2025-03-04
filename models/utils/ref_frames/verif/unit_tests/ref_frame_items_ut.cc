/*
 * ref_frame_items_ut.cc
 */

#include "utils/ref_frames/include/ref_frame_items.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RefFrameItems, create)
{
    RefFrameItems staticInst;
    RefFrameItems * dynInst = new RefFrameItems;
    delete dynInst;
}

TEST(RefFrameItems, to_string) {}
