/*
 * mass_body_init_ut.cc
 *
 *  Created on: June 6, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
#include "dynamics/body_action/include/mass_body_init.hh"
#include "mass_mock.hh"
#include "memory_interface_mock.hh"
#include "message_handler_mock.hh"
#include "simulation_interface_mock.hh"
#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::HasSubstr;
using testing::Mock;
using testing::Ref;
using testing::Return;
using namespace jeod;

class MassBodyInitTest : public MassBodyInit
{
public:
    unsigned int shutdown_count{};

    void shutdown() override
    {
        ++shutdown_count;
        BodyAction::shutdown();
    }

    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    MassBody * get_mass_subject()
    {
        return mass_subject;
    }
};

TEST(MassBodyInit, create)
{
    MockMessageHandler mockMessageHandler;
    {
        // points empty
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MassBodyInitTest * dynInst;
        dynInst = new MassBodyInitTest();
        MassBodyInit * dynInst2 = new MassBodyInit();
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_TRUE(dynInst->active);
        EXPECT_TRUE(dynInst->terminate_on_error);
        EXPECT_EQ(std::string(""), dynInst->action_name);

        EXPECT_EQ(std::string(""), dynInst->get_identifier());
        EXPECT_EQ(nullptr, dynInst->get_mass_subject());

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        delete dynInst2;
        delete dynInst;
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // points not empty
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MassBodyInitTest * dynInst = new MassBodyInitTest();
        MassPointInit * massPoint0 = new MassPointInit();
        MassPointInit * massPoint1 = new MassPointInit();
        Mock::VerifyAndClear(&mockMessageHandler);

        dynInst->points.push_back(massPoint0);
        dynInst->points.push_back(massPoint1);
        dynInst->points.push_back(nullptr);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        delete dynInst;
        Mock::VerifyAndClear(&mockMessageHandler);
    }
}

TEST(MassBodyInit, apply)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockMassBody mockMassBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    // Test mass init. Always succeeds
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MassBodyInitTest staticInst;
    staticInst.set_mass_subject(&mockMassBody);
    staticInst.apply(mockDynManager);
    Mock::VerifyAndClear(&mockMessageHandler);
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(MassBodyInit, allocate_points)
{
    MockMessageHandler mockMessageHandler;
    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);
    {
        // positive number of points
        ON_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).WillByDefault(Return(true));
        EXPECT_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).Times(2);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        MassBodyInitTest staticInst;
        int numPoints = 2;
        staticInst.allocate_points(numPoints);
        EXPECT_EQ(numPoints, staticInst.points.size());
        Mock::VerifyAndClear(&mockMemoryInterface);

        EXPECT_CALL(mockMemoryInterface, deregister_allocation(_, _, _, _, _)).Times(2);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(MassBodyInit, get_mass_point)
{
    MockMessageHandler mockMessageHandler;
    {
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MassBodyInitTest staticInst;
        MassPointInit * massPoint;
        MassPointInit * massPoint0 = new MassPointInit();
        MassPointInit * massPoint1 = new MassPointInit();
        Mock::VerifyAndClear(&mockMessageHandler);

        // empty check
        massPoint = staticInst.get_mass_point(1);
        EXPECT_EQ(nullptr, massPoint);

        // out of range check
        staticInst.points.push_back(massPoint0);
        massPoint = staticInst.get_mass_point(1);
        EXPECT_EQ(nullptr, massPoint);

        // normal check
        staticInst.points.push_back(massPoint1);
        massPoint = staticInst.get_mass_point(1);
        EXPECT_EQ(massPoint1, massPoint);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }
}

TEST(MassBodyInit, apply_dyn_subject_without_initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager dynManager;
    DynBody subject;
    MassBodyInitTest action;
    action.properties.mass = 42.0;
    action.set_subject_body(subject);
    ASSERT_EQ(nullptr, action.get_mass_subject());

    action.apply(dynManager);

    EXPECT_EQ(&subject.mass, action.get_mass_subject());
    EXPECT_DOUBLE_EQ(42.0, subject.mass.core_properties.mass);
    EXPECT_EQ(1u, action.shutdown_count);
}

TEST(MassBodyInit, apply_reassigned_dyn_subject)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager dynManager;
    MassBody original;
    DynBody replacement;
    MassBodyInitTest action;
    original.core_properties.mass = 7.0;
    action.set_subject_body(original);
    action.initialize(dynManager);
    action.set_subject_body(replacement);
    action.properties.mass = 21.0;

    action.apply(dynManager);

    EXPECT_EQ(&replacement.mass, action.get_mass_subject());
    EXPECT_DOUBLE_EQ(7.0, original.core_properties.mass);
    EXPECT_DOUBLE_EQ(21.0, replacement.mass.core_properties.mass);
    EXPECT_EQ(1u, action.shutdown_count);
}

TEST(MassBodyInit, apply_mass_subject_without_initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager dynManager;
    MassBody subject;
    MassBodyInitTest action;
    action.properties.mass = 14.0;
    action.set_subject_body(subject);

    action.apply(dynManager);

    EXPECT_DOUBLE_EQ(14.0, subject.core_properties.mass);
    EXPECT_EQ(1u, action.shutdown_count);
}

TEST(MassBodyInit, apply_missing_subject)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager dynManager;
    MassBodyInitTest action;
    EXPECT_CALL(mockMessageHandler,
                process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::null_pointer, _, _))
        .Times(1);

    action.apply(dynManager);

    EXPECT_EQ(nullptr, action.get_mass_subject());
    EXPECT_EQ(0u, action.shutdown_count);
}

TEST(MassBodyInit, apply_mismatched_subjects)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager dynManager;
    DynBody dynSubject;
    MassBody massSubject;
    MassBodyInitTest action;
    massSubject.core_properties.mass = 8.0;
    action.properties.mass = 16.0;
    action.set_subject_body(dynSubject);
    action.set_mass_subject(&massSubject);
    EXPECT_CALL(mockMessageHandler,
                process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::fatal_error, _, _))
        .Times(1);

    action.apply(dynManager);

    EXPECT_DOUBLE_EQ(8.0, massSubject.core_properties.mass);
    EXPECT_EQ(0u, action.shutdown_count);
}
