/*
 *  dyn_body_init_trans_state_ut.cc
 *
 *  Created on: June 27, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
#include "dynamics/body_action/include/dyn_body_init_trans_state.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
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

class DynBodyInitTransStateTest : public DynBodyInitTransState
{
public:
    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }

    RefFrame * get_body_ref_frame()
    {
        return body_ref_frame;
    }

    RefFrame * get_reference_ref_frame()
    {
        return reference_ref_frame;
    }

    const std::string get_reference_ref_frame_name()
    {
        return reference_ref_frame_name;
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

    void set_reference_ref_frame_name(const std::string & in)
    {
        reference_ref_frame_name = in;
    }

    void rotational()
    {
        this->compute_rotational_state();
    }

    DynBody * find_dyn_body(const DynManager & dyn_manager,
                            const std::string & dyn_body_name,
                            const std::string & variable_name)
    {
        return DynBodyInitTransState::find_dyn_body(dyn_manager, dyn_body_name, variable_name);
    }

    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return DynBodyInitTransState::find_planet(dyn_manager, planet_name, variable_name);
    }

    BodyRefFrame * find_body_frame(DynBody & frame_container,
                                   const std::string & body_frame_identifier,
                                   const std::string & variable_name)
    {
        return DynBodyInitTransState::find_body_frame(frame_container, body_frame_identifier, variable_name);
    }

    RefFrame * find_ref_frame(const DynManager & dyn_manager,
                              const std::string & body_frame_identifier,
                              const std::string & variable_name)
    {
        return DynBodyInitTransState::find_ref_frame(dyn_manager, body_frame_identifier, variable_name);
    }

    void apply_user_inputs()
    {
        DynBodyInitTransState::apply_user_inputs();
    }

    void compute_rotational_state()
    {
        DynBodyInitTransState::compute_rotational_state();
    }

    void compute_translational_state()
    {
        DynBodyInitTransState::compute_translational_state();
    }
};

class DynManagerTest : public DynManager
{
public:
    void set_dyn_bodies(const std::vector<DynBody *> & dyn_bodies_in)
    {
        dyn_bodies = dyn_bodies_in;
    }
};

TEST(DynBodyInitTransState, create)
{
    DynBodyInitTransState staticInst;
    DynBodyInitTransStateTest * dynInst;
    dynInst = new DynBodyInitTransStateTest();
    DynBodyInitTransState * dynInst2 = new DynBodyInitTransState();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_body_ref_frame());
    EXPECT_EQ(nullptr, dynInst->get_reference_ref_frame());

    delete dynInst2;
    delete dynInst;
}

TEST(DynBodyInitTransState, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    RefFrame refFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // nominal, state_items == both
        DynBodyInitTransStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&refFrame);
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // nominal, state_items == position
        DynBodyInitTransStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&refFrame);
        dynInst.state_items = DynBodyInitTransState::Position;
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // nominal, state_items == velocity
        DynBodyInitTransStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&refFrame);
        dynInst.state_items = DynBodyInitTransState::Velocity;
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // nominal, invalid state_items
        DynBodyInitTransStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&refFrame);
        dynInst.state_items = DynBodyInitTransState::StateItems(-1);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::illegal_value, _, _))
            .Times(1);
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(0);
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(0);
        dynInst.initialize(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInitTransState, initializes_what)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTransStateTest dynInst;
        dynInst.state_items = jeod::DynBodyInitTransState::StateItems::Both;
        dynInst.initializes_what();
    }
    {
        DynBodyInitTransStateTest dynInst;
        dynInst.state_items = jeod::DynBodyInitTransState::StateItems::Position;
        dynInst.initializes_what();
    }
    {
        DynBodyInitTransStateTest dynInst;
        dynInst.state_items = jeod::DynBodyInitTransState::StateItems::Velocity;
        dynInst.initializes_what();
    }
    {
        DynBodyInitTransStateTest dynInst;
        dynInst.state_items = jeod::DynBodyInitTransState::StateItems(-1);
        dynInst.initializes_what();
    }
}

TEST(DynBodyInitTransState, is_ready)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    BodyRefFrame bodyRefFrame;
    RefFrame referenceRefFrame;
    EphemerisRefFrame ephemRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Default test case with active and inactive status
        DynBodyInitTransStateTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        dynInst.active = false;
        dynInst.is_ready();
    }
    {
        DynBodyInitTransStateTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        dynInst.state_items = DynBodyInitTransState::Position;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        dynInst.is_ready();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // reference_ref_frame is BodyRefFrame
        DynBodyInitTransStateTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&bodyRefFrame);
        dynInst.is_ready();
    }
    {
        // reference_ref_frame is EphemerisRefFrame
        DynBodyInitTransStateTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&ephemRefFrame);
        dynInst.is_ready();
    }
}

TEST(DynBodyInitTransState, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.composite_body.set_owner(&mockDynBody);
    EphemerisRefFrame refFrame;
    refFrame.add_child(mockDynBody.composite_body);
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTransStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&refFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&refFrame));

        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
