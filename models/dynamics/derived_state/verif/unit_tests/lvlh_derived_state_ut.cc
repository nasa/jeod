/*
 * lvlh_derived_state_ut.cc
 *
 *  Created on: Dec 4, 2023
 *      Author: tbrain
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/derived_state/include/lvlh_derived_state.hh"
#include "message_handler_mock.hh"
#include "ref_frame_mock.hh"
#include "utils/ref_frames/include/ref_frame.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::HasSubstr;
using testing::Invoke;
using testing::Mock;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;

using namespace jeod;

class LvlhDerivedStateTest : public LvlhDerivedState
{
public:
    RefFrame * get_planet_centered_inertial()
    {
        return planet_centered_inertial;
    }

    void set_planet_centered_inertial(RefFrame * planet_centered_inertial_in)
    {
        planet_centered_inertial = planet_centered_inertial_in;
    }

    DynManager * get_local_dm()
    {
        return local_dm;
    }

    void set_local_dm(DynManager * local_dm_in)
    {
        local_dm = local_dm_in;
    }
};

TEST(LvlhDerivedState, create)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        LvlhDerivedState staticInst;
        LvlhDerivedStateTest * dynInst;
        dynInst = new LvlhDerivedStateTest();
        LvlhDerivedState * dynInst2 = new LvlhDerivedState;

        EXPECT_TRUE(dynInst->register_frame);
        EXPECT_EQ(nullptr, dynInst->get_planet_centered_inertial());
        EXPECT_EQ(nullptr, dynInst->get_local_dm());

        delete dynInst2;
        delete dynInst;
    }

    {
        // Destructor test, lvlh_frame should call remove_from_parent, no local_dm ptr
        MockRefFrame mockRefFrame;
        RefFrameLinks & links = mockRefFrame.get_links();
        LvlhDerivedStateTest * dynInst = new LvlhDerivedStateTest();

        mockRefFrame.RefFrame::add_child(dynInst->lvlh_frame);
        EXPECT_TRUE(links.has_children());
        EXPECT_TRUE(dynInst->lvlh_frame.is_progeny_of(mockRefFrame));
        delete dynInst;
        EXPECT_FALSE(links.has_children());
    }

    {
        // Destructor test, local_dm ptr is set and should call remove_ref_frame

        MockRefFrame mockRefFrame;
        LvlhDerivedStateTest * dynInst = new LvlhDerivedStateTest();
        dynInst->set_local_dm(&mockDynManager);
        EXPECT_CALL(mockDynManager, remove_ref_frame(Ref(dynInst->lvlh_frame))).Times(1);
        delete dynInst;
        Mock::VerifyAndClear(&mockDynManager);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(LvlhDerivedState, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("FooDynBody");
    mockDynBody.composite_body.set_name("FooDynBody.composite_body");
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Call initialize and check some refframe tree elements, no lvlh_frame register_frame
        BasePlanet basePlanet;
        LvlhDerivedStateTest staticInst;
        staticInst.register_frame = false;
        staticInst.reference_name = "FooPlanet";
        EXPECT_CALL(mockDynManager, find_base_planet(_)).Times(1).WillOnce(Return(&basePlanet));
        EXPECT_CALL(mockDynManager, add_ref_frame(Ref(staticInst.lvlh_state.frame))).Times(1);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, _, HasSubstr("no ephemerides manager"), _))
            .Times(1);

        staticInst.initialize(mockDynBody, mockDynManager);
        EXPECT_EQ(&mockDynBody, staticInst.subject);
        EXPECT_EQ(&basePlanet.inertial, staticInst.get_planet_centered_inertial());
        EXPECT_TRUE(staticInst.lvlh_frame.is_progeny_of(basePlanet.inertial));
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(AnyNumber());
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call initialize and check some refframe tree elements, call lvlh_frame register_frame
        BasePlanet basePlanet;
        LvlhDerivedStateTest staticInst;
        staticInst.register_frame = true;
        staticInst.reference_name = "FooPlanet";
        EXPECT_CALL(mockDynManager, find_base_planet(_)).Times(1).WillOnce(Return(&basePlanet));
        EXPECT_CALL(mockDynManager, add_ref_frame(Ref(staticInst.lvlh_state.frame))).Times(1);
        EXPECT_CALL(mockDynManager, add_ref_frame(Ref(staticInst.lvlh_frame))).Times(1);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, _, HasSubstr("no ephemerides manager"), _))
            .Times(1);

        staticInst.initialize(mockDynBody, mockDynManager);
        EXPECT_EQ(&mockDynBody, staticInst.subject);
        EXPECT_EQ(&basePlanet.inertial, staticInst.get_planet_centered_inertial());
        EXPECT_TRUE(staticInst.lvlh_frame.is_progeny_of(basePlanet.inertial));
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(AnyNumber());
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(LvlhDerivedState, update)
{
    MockMessageHandler mockMessageHandler;

    {
        // We aren't testing the LVLH calculations in this unit test. That belongs to the lvlh_frame_ut.cc test
        // Just check that the time stamps for the frames to be calculated are updated.
        BasePlanet basePlanet;
        RefFrame refFrame;

        basePlanet.inertial.add_child(refFrame);
        LvlhDerivedStateTest staticInst;
        EXPECT_EQ(0.0, staticInst.lvlh_state.frame.timestamp());
        EXPECT_EQ(0.0, staticInst.lvlh_frame.timestamp());
        staticInst.lvlh_state.set_planet(basePlanet);
        staticInst.lvlh_state.set_subject_frame(refFrame);
        refFrame.set_timestamp(123.0);
        staticInst.update();
        EXPECT_EQ(123.0, staticInst.lvlh_state.frame.timestamp());
        EXPECT_EQ(123.0, staticInst.lvlh_frame.timestamp());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
