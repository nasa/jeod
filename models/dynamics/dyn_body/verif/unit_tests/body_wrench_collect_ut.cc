/*
 * body_wrench_collect_ut.cc
 *
 *  Created on: Dec 5, 2023
 *      Author: tbrain
 */

#include "dynamics/dyn_body/include/body_wrench_collect.hh"
#include "message_handler_mock.hh"

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

TEST(BodyWrenchCollect, create)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    {
        BodyWrenchCollect staticInst;
        BodyWrenchCollect * dynInst;
        dynInst = new BodyWrenchCollect();

        EXPECT_EQ(0, dynInst->collect_wrench.size());

        delete dynInst;
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyWrenchCollect, accumulate_Wrench)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    {
        BodyWrenchCollect staticInst;
        double pos[3] = {1.0, 0.0, 0.0};
        double force[3] = {0.0, 1.0, 0.0};
        Wrench wrenches[3];
        wrenches[0].set_point(pos);
        wrenches[1].set_point(pos);
        wrenches[2].set_point(pos);
        wrenches[0].set_force(force);
        wrenches[1].set_force(force);
        wrenches[2].set_force(force);
        staticInst.collect_wrench.push_back(&wrenches[0]);
        staticInst.collect_wrench.push_back(&wrenches[1]);
        staticInst.collect_wrench.push_back(&wrenches[2]);
        EXPECT_EQ(3, staticInst.collect_wrench.size());
        Wrench srcWrench(pos);
        Wrench resWrench = staticInst.accumulate(srcWrench);

        double exp_force[3] = {0.0, 3.0, 0.0};
        double exp_torque[3] = {0.0, 0.0, 0.0};
        for(int ii = 0; ii < 3; ++ii)
        {
            EXPECT_EQ(exp_force[ii], resWrench.get_force()[ii]);
            EXPECT_EQ(exp_torque[ii], resWrench.get_torque()[ii]);
        }
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyWrenchCollect, accumulate_double_Wrench)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    {
        BodyWrenchCollect staticInst;
        double pos[3] = {1.0, 0.0, 0.0};
        double force[3] = {0.0, 1.0, 0.0};
        Wrench wrenches[3];
        wrenches[0].set_point(pos);
        wrenches[1].set_point(pos);
        wrenches[2].set_point(pos);
        wrenches[0].set_force(force);
        wrenches[1].set_force(force);
        wrenches[2].set_force(force);
        staticInst.collect_wrench.push_back(&wrenches[0]);
        staticInst.collect_wrench.push_back(&wrenches[1]);
        staticInst.collect_wrench.push_back(&wrenches[2]);
        EXPECT_EQ(3, staticInst.collect_wrench.size());
        Wrench srcWrench;
        Wrench resWrench = staticInst.accumulate(pos, srcWrench);

        double exp_force[3] = {0.0, 3.0, 0.0};
        double exp_torque[3] = {0.0, 0.0, 0.0};
        for(int ii = 0; ii < 3; ++ii)
        {
            EXPECT_EQ(exp_force[ii], resWrench.get_force()[ii]);
            EXPECT_EQ(exp_torque[ii], resWrench.get_torque()[ii]);
        }
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
