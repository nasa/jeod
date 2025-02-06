/*
 *  dyn_body_init_ut.cc
 *
 *  Created on: June 5, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
#include "dynamics/body_action/include/dyn_body_init.hh"
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

class DynBodyInitTest : public DynBodyInit
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
        return DynBodyInit::find_dyn_body(dyn_manager, dyn_body_name, variable_name);
    }

    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return DynBodyInit::find_planet(dyn_manager, planet_name, variable_name);
    }

    BodyRefFrame * find_body_frame(DynBody & frame_container,
                                   const std::string & body_frame_identifier,
                                   const std::string & variable_name)
    {
        return DynBodyInit::find_body_frame(frame_container, body_frame_identifier, variable_name);
    }

    RefFrame * find_ref_frame(const DynManager & dyn_manager,
                              const std::string & body_frame_identifier,
                              const std::string & variable_name)
    {
        return DynBodyInit::find_ref_frame(dyn_manager, body_frame_identifier, variable_name);
    }

    void apply_user_inputs()
    {
        DynBodyInit::apply_user_inputs();
    }

    void compute_rotational_state()
    {
        DynBodyInit::compute_rotational_state();
    }

    void compute_translational_state()
    {
        DynBodyInit::compute_translational_state();
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

TEST(DynBodyInit, create)
{
    DynBodyInit staticInst;
    DynBodyInitTest * dynInst;
    dynInst = new DynBodyInitTest();
    DynBodyInit * dynInst2 = new DynBodyInit();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_body_ref_frame());
    EXPECT_EQ(nullptr, dynInst->get_reference_ref_frame());

    delete dynInst2;
    delete dynInst;
}

TEST(DynBodyInit, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    MockRefFrame mockRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Null dyn_subject. Error
        DynBodyInitTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::invalid_object, _, _))
            .Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Body not registered with the dynamics manager. Error
        DynBodyInitTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(false));
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::invalid_object, _, _))
            .Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Null body ref frame, empty body_frame_id, null reference_ref_frame
        DynBodyInitTest dynInst;
        dynInst.reference_ref_frame_name = "ref_frame";
        dynInst.set_dyn_subject(&mockDynBody);
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&mockDynBody.composite_body));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&mockRefFrame));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Null body ref frame, valid body_frame_id, valid reference_ref_frame
        DynBodyInitTest dynInst;
        dynInst.set_reference_ref_frame(&mockRefFrame);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.body_frame_id = "composite_body";
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&mockDynBody.composite_body));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // valid body ref frame, valid reference_ref_frame
        DynBodyInitTest dynInst;
        dynInst.set_reference_ref_frame(&mockRefFrame);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMassBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, initializes_what)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        dynInst.initializes_what();
    }
}

TEST(DynBodyInit, is_ready)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    BodyRefFrame bodyRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Default test case with active and inactive status
        DynBodyInitTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        EXPECT_TRUE(dynInst.is_ready());
        dynInst.active = false;
        EXPECT_FALSE(dynInst.is_ready());
    }
    {
        DynBodyInitTest dynInst;
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.reverse_sense = true;
        EXPECT_FALSE(dynInst.is_ready());
    }
}

TEST(DynBodyInit, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.composite_body.set_owner(&mockDynBody);
    RefFrame referenceRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        dynInst.apply(mockDynManager);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, find_dyn_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Find dyn body success
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynManager, find_dyn_body(_)).Times(1).WillOnce(Return(&mockDynBody));
        dynInst.find_dyn_body(mockDynManager, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // Find dyn body failure
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynManager, find_dyn_body(_)).Times(1).WillOnce(Return(nullptr));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        dynInst.find_dyn_body(mockDynManager, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, report_failure)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    BodyRefFrame bodyRefFrame;
    RefFrame refFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Not null body ref frame
        DynBodyInitTest dynInst;
        dynInst.set_reference_ref_frame(&refFrame);
        dynInst.set_body_ref_frame(&bodyRefFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        dynInst.report_failure();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Null body ref frame
        DynBodyInitTest dynInst;
        dynInst.set_reference_ref_frame(&refFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        dynInst.report_failure();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, find_planet)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Planet planet;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        dynInst.find_planet(mockDynManager, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(nullptr));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        dynInst.find_planet(mockDynManager, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, apply_user_inputs)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    BodyRefFrame bodyRefFrame;
    RefFrame referenceRefFrame;
    EphemerisRefFrame testRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(2);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&testRefFrame));
        dynInst.apply_user_inputs();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        DynBodyInitTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        dynInst.reverse_sense = true;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(2);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&testRefFrame));
        dynInst.apply_user_inputs();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, compute_rotational_state)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    BodyRefFrame bodyRefFrame;
    RefFrame referenceRefFrame;
    EphemerisRefFrame testRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(4);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(2).WillRepeatedly(Return(&testRefFrame));
        dynInst.compute_rotational_state();
        dynInst.compute_rotational_state();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, compute_translational_state)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    BodyRefFrame bodyRefFrame;
    RefFrame referenceRefFrame;
    EphemerisRefFrame testRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&bodyRefFrame);
        dynInst.set_reference_ref_frame(&referenceRefFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(4);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(2).WillRepeatedly(Return(&testRefFrame));
        dynInst.compute_translational_state();
        dynInst.compute_translational_state();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, find_body_frame)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    BodyRefFrame bodyRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        dynInst.find_body_frame(mockDynBody, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(nullptr));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        dynInst.find_body_frame(mockDynBody, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInit, find_ref_frame)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    RefFrame testRefFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&testRefFrame));
        dynInst.find_ref_frame(mockDynManager, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        DynBodyInitTest dynInst;
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(nullptr));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        dynInst.find_ref_frame(mockDynManager, "test_name", "test_name");
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
