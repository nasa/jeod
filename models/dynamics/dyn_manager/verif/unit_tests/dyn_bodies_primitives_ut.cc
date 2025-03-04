/*
 * dyn_bodies_primitives_ut.cc
 */

#include "dyn_body_mock.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_manager/include/dyn_manager_messages.hh"
#include "dynamics/dyn_manager/verif/unit_tests/dyn_manager_ut.hh"
#include "memory_interface_mock.hh"
#include "message_handler_mock.hh"
#include "simulation_interface_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::HasSubstr;
using testing::Mock;

using namespace jeod;

TEST(DynManager, find_dyn_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Test empty string. Reports error and returns null
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManager staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        EXPECT_EQ(nullptr, staticInst.find_dyn_body(std::string("")));
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test valid strings. Finds and returns DynBody2, returns null for dynbody3
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        MockDynBody mockDynBody2;
        Mock::VerifyAndClear(&mockMessageHandler);

        mockDynBody.set_name("DynBody 1");
        mockDynBody2.set_name("DynBody 2");

        std::vector<DynBody *> dynBodiesListInput = {&mockDynBody, &mockDynBody2};
        staticInst.set_dyn_bodies(dynBodiesListInput);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(0);
        EXPECT_EQ(&mockDynBody2, staticInst.find_dyn_body(std::string("DynBody 2")));
        EXPECT_EQ(nullptr, staticInst.find_dyn_body(std::string("DynBody 3")));
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, is_dyn_body_registered)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        MockDynBody mockDynBody2;
        MockDynBody mockDynBody3;
        Mock::VerifyAndClear(&mockMessageHandler);

        std::vector<DynBody *> dynBodiesListInput = {&mockDynBody, &mockDynBody2};
        staticInst.set_dyn_bodies(dynBodiesListInput);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(0);
        EXPECT_TRUE(staticInst.is_dyn_body_registered(&mockDynBody2));
        EXPECT_FALSE(staticInst.is_dyn_body_registered(&mockDynBody3));
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, add_dyn_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Test adding unnamed dynbody. Reports error and fails to add.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        staticInst.add_dyn_body(mockDynBody);
        EXPECT_EQ(0, staticInst.get_dyn_bodies().size());
        EXPECT_EQ(0, staticInst.get_mass_bodies().size());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test adding duplicate dynbody. Reports error and fails to add.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        mockDynBody.set_name("TestDynBody");

        std::vector<DynBody *> dynBodiesListInput = {&mockDynBody};
        staticInst.set_dyn_bodies(dynBodiesListInput);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::duplicate_entry, _, _))
            .Times(1);
        staticInst.add_dyn_body(mockDynBody);
        EXPECT_EQ(1, staticInst.get_dyn_bodies().size());
        EXPECT_EQ(0, staticInst.get_mass_bodies().size());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test adding dynbody with duplicate name. Reports error and fails to add.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        MockDynBody mockDynBody2;
        Mock::VerifyAndClear(&mockMessageHandler);

        mockDynBody.set_name("TestDynBody");
        mockDynBody2.set_name("TestDynBody");

        std::vector<DynBody *> dynBodiesListInput = {&mockDynBody};
        staticInst.set_dyn_bodies(dynBodiesListInput);

        EXPECT_CALL(mockMessageHandler,
                    process_message(
                        MessageHandler::Error, _, _, _, DynManagerMessages::duplicate_entry, HasSubstr("with name"), _))
            .Times(1);
        staticInst.add_dyn_body(mockDynBody2);
        EXPECT_EQ(1, staticInst.get_dyn_bodies().size());
        EXPECT_EQ(0, staticInst.get_mass_bodies().size());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test adding dynbody with duplicate name as an added massbody of different body. Reports error and fails to
        // add.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        MassBody massBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        mockDynBody.set_name("TestBody");
        massBody.set_name("TestBody");

        std::vector<MassBody *> massBodiesListInput = {&massBody};
        staticInst.set_mass_bodies(massBodiesListInput);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::invalid_name, _, _))
            .Times(1);
        staticInst.add_dyn_body(mockDynBody);
        EXPECT_EQ(0, staticInst.get_dyn_bodies().size());
        EXPECT_EQ(1, staticInst.get_mass_bodies().size());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test adding dynbody with duplicate name as an added massbody of the same body. Reports warning but adds to
        // dynbody list.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        MassBody massBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        mockDynBody.set_name("TestBody");

        std::vector<MassBody *> massBodiesListInput = {&mockDynBody.mass};
        staticInst.set_mass_bodies(massBodiesListInput);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Warning, _, _, _, DynManagerMessages::duplicate_entry, _, _))
            .Times(1);
        staticInst.add_dyn_body(mockDynBody);
        EXPECT_EQ(1, staticInst.get_dyn_bodies().size());
        EXPECT_EQ(1, staticInst.get_mass_bodies().size());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Nominal add
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        mockDynBody.set_name("TestBody");

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.add_dyn_body(mockDynBody);
        EXPECT_EQ(1, staticInst.get_dyn_bodies().size());
        EXPECT_EQ(1, staticInst.get_mass_bodies().size());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
