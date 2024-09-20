/*
 *  dyn_body_init_ned_state_ut.cc
 *
 *  Created on: July 1, 2024
 *      Author: lmccartney
 */

#include "body_ref_frame_mock.hh"
#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
#include "dynamics/body_action/include/dyn_body_init_ned_state.hh"
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

class DynBodyInitNedStateTest : public DynBodyInitNedState
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

    bool get_body_is_required()
    {
        return body_is_required;
    }

    void set_planet(Planet * in)
    {
        planet = in;
    }

    void set_pfix_ptr(EphemerisRefFrame * in)
    {
        pfix_ptr = in;
    }

    EphemerisRefFrame * get_pfix_ptr()
    {
        return pfix_ptr;
    }
};

TEST(DynBodyInitNedState, create)
{
    DynBodyInitNedState * dynInst;
    dynInst = new DynBodyInitNedState();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    delete dynInst;
}

TEST(DynBodyInitNedState, initialize)
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
        // ref_body_name empty, use_alt_pfix true
        DynBodyInitNedStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.planet_name = "planet_name";
        dynInst.set_use_alt_pfix(true);
        dynInst.set_body_is_required(false);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_dyn_body(_)).Times(0);
        dynInst.initialize(mockDynManager);
        EXPECT_FALSE(dynInst.get_body_is_required());
        EXPECT_EQ(dynInst.get_pfix_ptr(), &planet.alt_pfix);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // valid ref_body_name, use_alt_pfix false
        DynBodyInitNedStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.ref_body_name = "mockDynBody";
        dynInst.planet_name = "planet_name";
        dynInst.set_body_is_required(false);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_dyn_body(_)).Times(1).WillOnce(Return(&mockDynBody));
        dynInst.initialize(mockDynManager);
        EXPECT_TRUE(dynInst.get_body_is_required());
        EXPECT_EQ(dynInst.get_pfix_ptr(), &planet.pfix);
        EXPECT_TRUE(dynInst.get_body_is_required());
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInitNedState, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("mockDynBody");
    mockDynBody.composite_body.set_name("mockDynBody", "composite_body");
    mockDynBody.composite_body.set_owner(&mockDynBody);
    Planet planet;
    planet.name = "planet";
    planet.pfix.set_name("planet", "pfix");
    planet.inertial.set_name("planet", "inertial");
    planet.inertial.add_child(mockDynBody.composite_body);
    planet.inertial.add_child(planet.pfix);
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // ref_body not null
        DynBodyInitNedStateTest dynInst;
        dynInst.planet_name = "planet_name";
        dynInst.altlatlong_type = NorthEastDown::spherical;
        dynInst.set_pfix_ptr(&planet.pfix);
        dynInst.set_ref_body(&mockDynBody);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, get_integ_frame()).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // ref_body null, case NorthEastDown::undefined
        DynBodyInitNedStateTest dynInst;
        dynInst.altlatlong_type = NorthEastDown::undefined;
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::illegal_value, _, _))
            .Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // valid ref_body, case NorthEastDown::spherical
        DynBodyInitNedStateTest dynInst;
        dynInst.planet_name = "planet_name";
        dynInst.altlatlong_type = NorthEastDown::spherical;
        dynInst.set_pfix_ptr(&planet.pfix);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, get_integ_frame()).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // ref_body null, case NorthEastDown::elliptical
        DynBodyInitNedStateTest dynInst;
        dynInst.planet_name = "planet_name";
        dynInst.altlatlong_type = NorthEastDown::elliptical;
        dynInst.set_pfix_ptr(&planet.pfix);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, get_integ_frame()).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInitNedState, set_use_alt_pfix)
{
    DynBodyInitNedStateTest dynInst;

    dynInst.set_use_alt_pfix(true);
    EXPECT_TRUE(dynInst.get_use_alt_pfix());

    dynInst.set_use_alt_pfix(false);
    EXPECT_FALSE(dynInst.get_use_alt_pfix());
}
