/*
 *  dyn_body_frame_switch_ut.cc
 *
 *  Created on: June 25, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/dyn_body_frame_switch.hh"
#include "gravity_controls_mock.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"
#include "ref_frame_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::Mock;
using testing::Ref;
using testing::Return;
using namespace jeod;

class DynBodyFrameSwitchTest : public DynBodyFrameSwitch
{
public:
    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }

    MassBody * get_mass_subject()
    {
        return mass_subject;
    }

    EphemerisRefFrame * get_integ_frame()
    {
        return integ_frame;
    }

    void set_dyn_subject(DynBody * in)
    {
        dyn_subject = in;
    }

    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    void set_integ_frame(EphemerisRefFrame * in)
    {
        integ_frame = in;
    }
};

TEST(DynBodyFrameSwitch, create)
{
    DynBodyFrameSwitch staticInst;
    DynBodyFrameSwitchTest * dynInst;
    dynInst = new DynBodyFrameSwitchTest();
    DynBodyFrameSwitch * dynInst2 = new DynBodyFrameSwitch();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_mass_subject());

    delete dynInst2;
    delete dynInst;
}

TEST(DynBodyFrameSwitch, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    EphemerisRefFrame ephemerisRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Null dyn_subject
        DynBodyFrameSwitchTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Empty integ_frame_name
        DynBodyFrameSwitchTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // find_integ_frame returns nullptr
        DynBodyFrameSwitchTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.integ_frame_name = "test_name";
        EXPECT_CALL(mockDynManager, find_integ_frame(_)).Times(1).WillOnce(Return(nullptr));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // Reaches end of initialization
        DynBodyFrameSwitchTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.integ_frame_name = "test_name";
        EXPECT_CALL(mockDynManager, find_integ_frame(_)).Times(1).WillOnce(Return(&ephemerisRefFrame));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyFrameSwitch, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockGravityControls mockControls[2];
    mockDynBody.grav_interaction.add_control(&mockControls[0]);
    mockDynBody.grav_interaction.add_control(&mockControls[1]);
    EphemerisRefFrame integFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // sort_grav_controls is false
        DynBodyFrameSwitchTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.integ_frame_name = "test_name";
        dynInst.sort_grav_controls = false;
        dynInst.set_integ_frame(&integFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, switch_integration_frames(_)).Times(1);
        EXPECT_CALL(mockControls[0], gravitation(mockDynBody.composite_body.state.trans.position, _, _, _, _)).Times(0);
        EXPECT_CALL(mockControls[1], gravitation(mockDynBody.composite_body.state.trans.position, _, _, _, _)).Times(0);

        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockControls[0]);
        Mock::VerifyAndClear(&mockControls[1]);
    }
    {
        // sort_grav_controls is true
        DynBodyFrameSwitchTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.integ_frame_name = "test_name";
        dynInst.sort_grav_controls = true;
        dynInst.set_integ_frame(&integFrame);
        mockControls[1].active = true;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, switch_integration_frames(_)).Times(1);
        EXPECT_CALL(mockControls[0], gravitation(mockDynBody.composite_body.state.trans.position, _, _, _, _)).Times(0);
        EXPECT_CALL(mockControls[1], gravitation(mockDynBody.composite_body.state.trans.position, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyFrameSwitch, is_ready)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    EphemerisRefFrame integFrame;
    integFrame.add_child(mockDynBody.composite_body);
    mockDynBody.composite_body.state.trans.position[0] = 5.0;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // active, switchonapproach, too far away
        DynBodyFrameSwitchTest dynInst;
        dynInst.switch_sense = DynBodyFrameSwitch::SwitchOnApproach;
        dynInst.switch_distance = 7.5;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_integ_frame(&integFrame);
        EXPECT_TRUE(dynInst.is_ready());
    }
    {
        // active, switchonapproach, close enough
        DynBodyFrameSwitchTest dynInst;
        dynInst.switch_sense = DynBodyFrameSwitch::SwitchOnApproach;
        dynInst.switch_distance = 4.0;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_integ_frame(&integFrame);
        EXPECT_FALSE(dynInst.is_ready());
    }
    {
        // active, switchondeparture, too close
        DynBodyFrameSwitchTest dynInst;
        dynInst.switch_sense = DynBodyFrameSwitch::SwitchOnDeparture;
        dynInst.switch_distance = 7.5;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_integ_frame(&integFrame);
        EXPECT_FALSE(dynInst.is_ready());
    }
    {
        // active, switchondeparture, far enough
        DynBodyFrameSwitchTest dynInst;
        dynInst.switch_sense = DynBodyFrameSwitch::SwitchOnDeparture;
        dynInst.switch_distance = 4.0;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_integ_frame(&integFrame);
        EXPECT_TRUE(dynInst.is_ready());
    }
    {
        // inactive
        DynBodyFrameSwitchTest dynInst;
        dynInst.active = false;
        dynInst.switch_sense = DynBodyFrameSwitch::SwitchOnDeparture;
        dynInst.switch_distance = 4.0;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_integ_frame(&integFrame);
        EXPECT_FALSE(dynInst.is_ready());
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
