/*
 *  dyn_body_init_planet_derived_ut.cc
 *
 *  Created on: June 25, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/dyn_body_init_planet_derived.hh"
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

class DynBodyInitPlanetDerivedTest : public DynBodyInitPlanetDerived
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
};

TEST(DynBodyInitPlanetDerived, create)
{
    DynBodyInitPlanetDerived * dynInst;
    dynInst = new DynBodyInitPlanetDerived();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    delete dynInst;
}

TEST(DynBodyInitPlanetDerived, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    BodyRefFrame bodyRefFrame;
    Planet planet;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // ref_body null, body_is_required true
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.ref_body_name = "mockDynBody";
        dynInst.planet_name = "planet_name";
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_dyn_body(_)).Times(1).WillOnce(Return(&mockDynBody));
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // ref_body not null, body_is_required true
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.ref_body_name = "mockDynBody";
        dynInst.planet_name = "planet_name";
        dynInst.set_ref_body(&mockDynBody);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_dyn_body(_)).Times(0);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // ref_body null, body_is_required false
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.ref_body_name = "mockDynBody";
        dynInst.planet_name = "planet_name";
        dynInst.set_ref_body(&mockDynBody);
        dynInst.set_body_is_required(false);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_dyn_body(_)).Times(0);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInitPlanetDerived, is_ready)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    BodyRefFrame bodyRefFrame;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // DynBodyInitWrtPlanet::is_ready() true, ref_body null
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        EXPECT_TRUE(dynInst.is_ready());
    }
    {
        // DynBodyInitWrtPlanet::is_ready() true, ref_body not null
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_ref_body(&mockDynBody);
        EXPECT_FALSE(dynInst.is_ready());
    }
    {
        // DynBodyInitWrtPlanet::is_ready() false, ref_body null
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.active = false;
        EXPECT_FALSE(dynInst.is_ready());
    }
    {
        // DynBodyInitWrtPlanet::is_ready() false, ref_body not null
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_ref_body(&mockDynBody);
        dynInst.active = false;
        EXPECT_FALSE(dynInst.is_ready());
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInitPlanetDerived, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.composite_body.set_owner(&mockDynBody);
    RefFrame referenceRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitPlanetDerivedTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
