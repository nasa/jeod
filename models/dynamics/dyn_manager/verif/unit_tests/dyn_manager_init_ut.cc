/*
 * dyn_manager_init_ut.cc
 *
 *  Created on: Feb 6, 2024
 *      Author: tbrain
 */

#include "dynamics/dyn_manager/include/dyn_manager_init.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::HasSubstr;
using testing::Mock;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;

using namespace jeod;

TEST(DynManagerInit, create)
{
    DynManagerInit staticInst;
    DynManagerInit * dynInst = new DynManagerInit();

    EXPECT_EQ(DynManagerInit::EphemerisMode_Ephemerides, dynInst->mode);
    EXPECT_EQ(std::string(""), dynInst->central_point_name);
    EXPECT_EQ(nullptr, dynInst->integ_group_constructor);
    EXPECT_EQ(nullptr, dynInst->integ_constructor);
    EXPECT_EQ(er7_utils::Integration::Unspecified, dynInst->jeod_integ_opt);
    EXPECT_EQ(-1, dynInst->sim_integ_opt);

    delete dynInst;
}
