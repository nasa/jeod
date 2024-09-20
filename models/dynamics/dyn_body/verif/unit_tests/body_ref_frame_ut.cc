/*
 * body_ref_frame_ut.cc
 *
 *  Created on: Nov 30, 2023
 *      Author: tbrain
 */

#include "dynamics/dyn_body/include/body_ref_frame.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::Mock;
using testing::Return;
using testing::ReturnRef;

using namespace jeod;

TEST(BodyRefFrame, create)
{
    BodyRefFrame staticInst;
    BodyRefFrame * dynInst;
    dynInst = new BodyRefFrame();

    EXPECT_EQ(RefFrameItems::No_Items, dynInst->initialized_items.value);
    EXPECT_EQ(nullptr, dynInst->mass_point);

    delete dynInst;
}
