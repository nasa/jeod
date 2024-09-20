/*
 *  dyn_body_init_wrt_planet_ut.cc
 *
 *  Created on: June 25, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/dyn_body_init_wrt_planet.hh"
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

class DynBodyInitWrtPlanetTest : public DynBodyInitWrtPlanet
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
};

TEST(DynBodyInitWrtPlanet, create)
{
    DynBodyInitWrtPlanet * dynInst;
    dynInst = new DynBodyInitWrtPlanet();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    delete dynInst;
}

TEST(DynBodyInitWrtPlanet, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Planet planet;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitWrtPlanetTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.planet_name = "planet_name";
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        EXPECT_EQ(dynInst.get_reference_ref_frame(), &planet.inertial);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInitWrtPlanet, initializes_what)
{
    DynBodyInitWrtPlanetTest dynInst;
    dynInst.initializes_what();
}

TEST(DynBodyInitWrtPlanet, is_ready)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    BodyRefFrame bodyRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Default test case with active and inactive status
        DynBodyInitWrtPlanetTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        EXPECT_TRUE(dynInst.is_ready());
        dynInst.active = false;
        EXPECT_FALSE(dynInst.is_ready());
    }
    {
        DynBodyInitWrtPlanetTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.reverse_sense = true;
        EXPECT_TRUE(dynInst.is_ready());
    }
}

TEST(DynBodyInitWrtPlanet, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    BodyRefFrame bodyRefFrame;
    RefFrame referenceRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitWrtPlanetTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
