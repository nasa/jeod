/*
 * body_attach_matrix_ut.cc
 *
 *  Created on: June 6, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_attach_matrix.hh"
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

class BodyAttachMatrixTest : public BodyAttachMatrix
{
public:
    void set_dyn_subject(DynBody * in)
    {
        dyn_subject = in;
    }

    void set_dyn_parent(DynBody * in)
    {
        dyn_parent = in;
    }

    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    void set_mass_parent(MassBody * in)
    {
        mass_parent = in;
    }

    void set_ref_parent(RefFrame * in)
    {
        ref_parent = in;
    }

    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }

    DynBody * get_dyn_parent()
    {
        return dyn_parent;
    }

    MassBody * get_mass_subject()
    {
        return mass_subject;
    }

    MassBody * get_mass_parent()
    {
        return mass_parent;
    }

    RefFrame * get_ref_parent()
    {
        return ref_parent;
    }
};

TEST(BodyAttachMatrix, create)
{
    BodyAttachMatrix staticInst;
    BodyAttachMatrixTest * dynInst;
    dynInst = new BodyAttachMatrixTest();
    BodyAttachMatrix * dynInst2 = new BodyAttachMatrix();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(std::string(""), dynInst->get_identifier());
    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_mass_subject());
    EXPECT_EQ(nullptr, dynInst->get_mass_parent());
    EXPECT_EQ(nullptr, dynInst->get_dyn_parent());
    EXPECT_EQ(nullptr, dynInst->get_ref_parent());
    delete dynInst2;
    delete dynInst;
}

TEST(BodyAttachMatrix, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockDynBody mockDynParent;
    BodyRefFrame bodyRefFrame;
    MockMassBody mockMassBody;
    MockMassBody mockMassParent;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // dyn_parent not null, dyn_subject not null branch
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        BodyAttachMatrixTest dynInst;
        dynInst.set_dyn_parent(&mockDynParent);
        dynInst.set_dyn_subject(&mockDynBody);
        EXPECT_CALL(mockDynParent, attach_child(An<const double *>(), An<const double(*)[3]>(), Ref(mockDynBody)))
            .Times(1)
            .WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynParent);
    }
    {
        // dyn_parent not null, dyn_subject null branch
        BodyAttachMatrixTest dynInst;
        dynInst.set_dyn_parent(&mockDynParent);
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynParent, add_mass_body(An<const double *>(), An<const double(*)[3]>(), Ref(mockMassBody)))
            .Times(1)
            .WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynParent);
    }

    {
        // mass_parent not null, dyn_subject not null branch
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachMatrixTest dynInst;
        dynInst.set_mass_parent(&mockMassParent);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // mass_parent not null, dyn_subject null branch
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        BodyAttachMatrixTest dynInst;
        dynInst.set_mass_parent(&mockMassParent);
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockMassBody, attach_to(An<double *>(), An<double(*)[3]>(), _)).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // ref_parent not null, dyn_subject not null branch
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        BodyAttachMatrixTest dynInst;
        dynInst.set_ref_parent(&bodyRefFrame);
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // ref_parent not null, dyn_subject null branch
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachMatrixTest dynInst;
        dynInst.set_ref_parent(&bodyRefFrame);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // else branch
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachMatrixTest dynInst;
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
