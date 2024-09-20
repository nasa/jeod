/*
 * euler_derived_state_ut.cc
 *
 *  Created on: Dec 1, 2023
 *      Author: tbrain
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/derived_state/include/euler_derived_state.hh"
#include "message_handler_mock.hh"
#include "ref_frame_mock.hh"
#include "utils/ref_frames/include/ref_frame.hh"

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

class EulerDerivedStateTest : public EulerDerivedState
{
public:
    RefFrame * get_rel_frame()
    {
        return rel_frame;
    }

    void set_rel_frame(RefFrame * rel_frame_in)
    {
        rel_frame = rel_frame_in;
    }
};

TEST(EulerDerivedState, create)
{
    {
        EulerDerivedState staticInst;
        EulerDerivedStateTest * dynInst;
        dynInst = new EulerDerivedStateTest();
        EulerDerivedState * dynInst2 = new EulerDerivedState;

        EXPECT_EQ(Orientation::Roll_Pitch_Yaw, dynInst->sequence);
        EXPECT_EQ(nullptr, dynInst->get_rel_frame());

        for(int ii = 0; ii < 3; ++ii)
        {
            EXPECT_EQ(0.0, dynInst->ref_body_angles[ii]);
            EXPECT_EQ(0.0, dynInst->body_ref_angles[ii]);
        }

        // No rel_frame assigned, no call to unsubscribe
        delete dynInst2;
        delete dynInst;
    }

    {
        // Destructor test, rel_frame assigned should call unsubscribe
        RefFrame refFrame;
        refFrame.subscribe();
        EXPECT_EQ(1, refFrame.subscriptions());
        EulerDerivedStateTest * dynInst = new EulerDerivedStateTest();
        dynInst->set_rel_frame(&refFrame);
        delete dynInst;
        EXPECT_EQ(0, refFrame.subscriptions());
    }
}

TEST(EulerDerivedState, initialize_DynBody_DynManager)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("TestDynBody");
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        EulerDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.initialize(mockDynBody, mockDynManager);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(EulerDerivedState, initialize_RefFrame_DynBody_DynManager)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("TestDynBody");
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        RefFrame refFrame;
        EulerDerivedStateTest staticInst;
        staticInst.reference_name = "refname";
        staticInst.initialize(refFrame, mockDynBody, mockDynManager);
        EXPECT_EQ(&refFrame, staticInst.get_rel_frame());
        EXPECT_EQ(1, refFrame.subscriptions());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(EulerDerivedState, update)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);
        // Compute body wrt inertial, no rel_frame set
        double exp_ref_body_angles[3] = {30, 45.0, 60.0};
        double exp_body_ref_angles[3] = {-51.8765682554021907, 7.2862451871156360, -69.1187903196461093};
        double matrix[3][3] = {
            { 0.3535533905932738,  0.9267766952966369, 0.1268264840443220},
            {-0.6123724356957946,  0.1268264840443223, 0.7803300858899106},
            { 0.7071067811865475, -0.3535533905932737, 0.6123724356957946}
        };
        EulerDerivedStateTest staticInst;
        staticInst.subject = &mockDynBody;
        for(int ii = 0; ii < 3; ++ii)
        {
            for(int jj = 0; jj < 3; ++jj)
            {
                mockDynBody.composite_body.state.rot.T_parent_this[ii][jj] = matrix[ii][jj];
            }
        }
        staticInst.update();
        for(int ii = 0; ii < 3; ++ii)
        {
            double expValInRads = exp_ref_body_angles[ii] * M_PI / 180.0;
            EXPECT_DOUBLE_EQ(expValInRads, staticInst.ref_body_angles[ii]);
        }
        for(int ii = 0; ii < 3; ++ii)
        {
            double expValInRads = exp_body_ref_angles[ii] * M_PI / 180.0;
            EXPECT_DOUBLE_EQ(expValInRads, staticInst.body_ref_angles[ii]);
        }

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Compute body wrt parent, rel_frame must be parent
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        double exp_ref_body_angles[3] = {30, 45.0, 60.0};
        double exp_body_ref_angles[3] = {-51.8765682554021907, 7.2862451871156360, -69.1187903196461093};
        double matrix[3][3] = {
            { 0.3535533905932738,  0.9267766952966369, 0.1268264840443220},
            {-0.6123724356957946,  0.1268264840443223, 0.7803300858899106},
            { 0.7071067811865475, -0.3535533905932737, 0.6123724356957946}
        };
        RefFrame refFrame;
        EulerDerivedStateTest staticInst;
        staticInst.subject = &mockDynBody;
        staticInst.set_rel_frame(&refFrame);

        refFrame.add_child(mockDynBody.composite_body);

        for(int ii = 0; ii < 3; ++ii)
        {
            for(int jj = 0; jj < 3; ++jj)
            {
                mockDynBody.composite_body.state.rot.T_parent_this[ii][jj] = matrix[ii][jj];
            }
        }
        staticInst.update();
        for(int ii = 0; ii < 3; ++ii)
        {
            double expValInRads = exp_ref_body_angles[ii] * M_PI / 180.0;
            EXPECT_DOUBLE_EQ(expValInRads, staticInst.ref_body_angles[ii]);
        }
        for(int ii = 0; ii < 3; ++ii)
        {
            double expValInRads = exp_body_ref_angles[ii] * M_PI / 180.0;
            EXPECT_DOUBLE_EQ(expValInRads, staticInst.body_ref_angles[ii]);
        }
        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Compute body wrt another frame, rel_frame is set but is not the parent
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        // Compute body wrt parent, rel_frame must be parent
        double exp_ref_body_angles[3] = {30, 45.0, 60.0};
        double exp_body_ref_angles[3] = {-51.8765682554021907, 7.2862451871156360, -69.1187903196461093};
        double matrix[3][3] = {
            { 0.3535533905932738,  0.9267766952966369, 0.1268264840443220},
            {-0.6123724356957946,  0.1268264840443223, 0.7803300858899106},
            { 0.7071067811865475, -0.3535533905932737, 0.6123724356957946}
        };
        RefFrame refFrame;
        RefFrame refFrame2;
        EulerDerivedStateTest staticInst;
        staticInst.subject = &mockDynBody;
        staticInst.set_rel_frame(&refFrame);

        refFrame.add_child(refFrame2);
        refFrame2.add_child(mockDynBody.composite_body);

        for(int ii = 0; ii < 3; ++ii)
        {
            for(int jj = 0; jj < 3; ++jj)
            {
                mockDynBody.composite_body.state.rot.T_parent_this[ii][jj] = matrix[ii][jj];
            }
        }
        staticInst.update();
        for(int ii = 0; ii < 3; ++ii)
        {
            double expValInRads = exp_ref_body_angles[ii] * M_PI / 180.0;
            EXPECT_DOUBLE_EQ(expValInRads, staticInst.ref_body_angles[ii]);
        }
        for(int ii = 0; ii < 3; ++ii)
        {
            double expValInRads = exp_body_ref_angles[ii] * M_PI / 180.0;
            EXPECT_DOUBLE_EQ(expValInRads, staticInst.body_ref_angles[ii]);
        }
        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
