/*
 *  dyn_body_init_lvlh_state_ut.cc
 *
 *  Created on: June 25, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/dyn_body_init_lvlh_state.hh"
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

class DynBodyInitLvlhStateTest : public DynBodyInitLvlhState
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

    void set_planet(Planet * in)
    {
        planet = in;
    }

    void set_body_is_required(bool state)
    {
        body_is_required = state;
    }
};

TEST(DynBodyInitLvlhState, create)
{
    DynBodyInitLvlhState * dynInst;
    dynInst = new DynBodyInitLvlhState();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    delete dynInst;
}

TEST(DynBodyInitLvlhState, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Planet planet;
    LvlhFrame lvlhFrame;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // null lvlh_object_ptr
        DynBodyInitLvlhStateTest dynInst;
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
        // valid lvlh_object_ptr
        DynBodyInitLvlhStateTest dynInst;
        dynInst.set_lvlh_frame_object(lvlhFrame);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.ref_body_name = "mockDynBody";
        dynInst.planet_name = "planet_name";
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

TEST(DynBodyInitLvlhState, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("mockDynBody");
    mockDynBody.composite_body.set_name("mockDynBody", "composite_body");
    mockDynBody.composite_body.set_owner(&mockDynBody);
    Planet testPlanet;
    testPlanet.set_name("testPlanet");
    testPlanet.inertial.set_name("testPlanet", "inertial");
    testPlanet.inertial.set_ephem_manager(&mockDynManager);
    testPlanet.inertial.add_child(mockDynBody.composite_body);

    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // null lvlh_object_ptr, Rectilinear
        DynBodyInitLvlhStateTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_ref_body(&mockDynBody);
        dynInst.set_planet(&testPlanet);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, add_ref_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, ephem_note_tree_status_change()).Times(2);
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(1);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&testPlanet.inertial));

        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // valid lvlh_object_ptr, CircularCurvilinear, no orientation sequence
        LvlhFrame lvlhFrame;
        lvlhFrame.set_subject_frame(mockDynBody.composite_body);
        lvlhFrame.set_planet(testPlanet);
        DynBodyInitLvlhStateTest dynInst;
        dynInst.lvlh_type = LvlhType::CircularCurvilinear;
        dynInst.set_lvlh_frame_object(lvlhFrame);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_ref_body(&mockDynBody);
        dynInst.set_planet(&testPlanet);
        EXPECT_CALL(mockDynManager, ephem_note_tree_status_change()).Times(1);
        EXPECT_CALL(mockDynManager, add_ref_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&testPlanet.inertial));

        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, ephem_note_tree_status_change()).Times(1);
    }
    {
        // valid lvlh_object_ptr, CircularCurvilinear, orientation sequence Roll_Pitch_Yaw
        LvlhFrame lvlhFrame;
        lvlhFrame.set_subject_frame(mockDynBody.composite_body);
        lvlhFrame.set_planet(testPlanet);
        DynBodyInitLvlhStateTest dynInst;
        dynInst.lvlh_type = LvlhType::CircularCurvilinear;
        dynInst.orientation.euler_sequence = Orientation::Roll_Pitch_Yaw;
        dynInst.set_lvlh_frame_object(lvlhFrame);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_ref_body(&mockDynBody);
        dynInst.set_planet(&testPlanet);
        EXPECT_CALL(mockDynManager, add_ref_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, ephem_note_tree_status_change()).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&testPlanet.inertial));

        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, ephem_note_tree_status_change()).Times(1);
    }
    {
        // valid lvlh_object_ptr, EllipticalCurvilinear
        LvlhFrame lvlhFrame;
        lvlhFrame.set_subject_frame(mockDynBody.composite_body);
        lvlhFrame.set_planet(testPlanet);
        DynBodyInitLvlhStateTest dynInst;
        dynInst.lvlh_type = LvlhType::EllipticalCurvilinear;
        testPlanet.inertial.add_child(lvlhFrame.frame);
        dynInst.set_lvlh_frame_object(lvlhFrame);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_ref_body(&mockDynBody);
        dynInst.set_planet(&testPlanet);
        EXPECT_CALL(mockDynManager, add_ref_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, ephem_note_tree_status_change()).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(2);

        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, ephem_note_tree_status_change()).Times(1);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
