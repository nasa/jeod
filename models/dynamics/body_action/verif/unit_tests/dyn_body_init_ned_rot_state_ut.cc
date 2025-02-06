/*
 *  dyn_body_init_ned_rot_state_ut.cc
 *
 *  Created on: July 1, 2024
 *      Author: lmccartney
 */

#include "body_ref_frame_mock.hh"
#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/dyn_body_init_ned_rot_state.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::Mock;
using testing::Ref;
using testing::Return;
using namespace jeod;

class DynBodyInitNedRotStateTest : public DynBodyInitNedRotState
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

    RefFrame * get_body_ref_frame()
    {
        return body_ref_frame;
    }

    RefFrame * get_reference_ref_frame()
    {
        return reference_ref_frame;
    }

    DynBody * get_ref_body()
    {
        return ref_body;
    }

    const bool get_use_alt_pfix()
    {
        return use_alt_pfix;
    }

    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    void set_dyn_subject(DynBody * in)
    {
        dyn_subject = in;
    }

    void set_body_ref_frame(BodyRefFrame * in)
    {
        body_ref_frame = in;
    }

    void set_reference_ref_frame(RefFrame * in)
    {
        reference_ref_frame = in;
    }

    void set_ref_body(DynBody * in)
    {
        ref_body = in;
    }

    void set_body_is_required(bool state)
    {
        body_is_required = state;
    }

    void set_planet(Planet * in)
    {
        planet = in;
    }

    void set_pfix_ptr(EphemerisRefFrame * in)
    {
        pfix_ptr = in;
    }
};

TEST(DynBodyInitNedRotState, create)
{
    DynBodyInitNedRotState * dynInst;
    dynInst = new DynBodyInitNedRotState();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    delete dynInst;
}

TEST(DynBodyInitNedRotState, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("mockDynBody");
    mockDynBody.composite_body.set_name("mockDynBody", "composite_body");
    mockDynBody.composite_body.set_owner(&mockDynBody);
    Planet planet;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // nominal
        DynBodyInitNedRotStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.planet_name = "planet_name";
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // set_items invalid (contains Pos)
        DynBodyInitNedRotStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.planet_name = "planet_name";
        dynInst.set_items = RefFrameItems::Pos_Att_Rate;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        RefFrameItems test_items(dynInst.set_items);
        EXPECT_TRUE(test_items.contains(RefFrameItems::Att_Rate));
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // set_items invalid (contains Vel)
        DynBodyInitNedRotStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.planet_name = "planet_name";
        dynInst.set_items = RefFrameItems::Vel_Att_Rate;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        RefFrameItems test_items(dynInst.set_items);
        EXPECT_TRUE(test_items.contains(RefFrameItems::Att_Rate));
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
