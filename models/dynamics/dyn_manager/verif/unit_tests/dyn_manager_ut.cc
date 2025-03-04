/*
 * dyn_manager_ut.cc
 *
 *  Created on: Dec 13, 2023
 *      Author: tbrain
 */

#include "dynamics/dyn_manager/verif/unit_tests/dyn_manager_ut.hh"
#include "body_action_mock.hh"
#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_manager/include/dyn_manager_messages.hh"
#include "dynamics_integration_group_mock.hh"
#include "environment/gravity/include/gravity_manager.hh"
#include "integrator_constructor_mock.hh"
#include "jeod_integration_time_mock.hh"
#include "jeod_integrator_interface_mock.hh"
#include "memory_interface_mock.hh"
#include "message_handler_mock.hh"
#include "simple_ephemerides_mock.hh"
#include "simulation_interface_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::HasSubstr;
using testing::Mock;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;

using namespace jeod;

TEST(DynManager, create)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManager staticInst;
        DynManagerTest * dynInst;
        dynInst = new DynManagerTest();
        DynManager * dynInst2 = new DynManager();
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_FALSE(dynInst->deriv_ephem_update);
        EXPECT_FALSE(dynInst->gravity_off);
        EXPECT_EQ(DynManagerInit::EphemerisMode_Ephemerides, dynInst->mode);
        EXPECT_EQ(nullptr, dynInst->sim_integrator);

        EXPECT_FALSE(dynInst->is_initialized());
        EXPECT_EQ(nullptr, dynInst->get_gravity_manager());
        EXPECT_EQ(nullptr, dynInst->get_integ_constructor());
        EXPECT_EQ(nullptr, dynInst->get_integ_interface());
        EXPECT_EQ(nullptr, dynInst->get_default_integ_group());
        EXPECT_EQ(nullptr, dynInst->get_simple_ephemeris());

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, HasSubstr("master memory manager"), _))
            .Times(AnyNumber());
        delete dynInst2;
        delete dynInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Destructor test with standard-allocated params
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest * dynInst = new DynManagerTest();
        DynamicsIntegrationGroup dynIntegrationGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        MockSinglePointEphemeris mockSinglePointEphemeris;
        dynInst->set_simple_ephemeris(&mockSinglePointEphemeris);

        MockJeodIntegratorInterface mockJeodIntegratorInterface;
        dynInst->set_integ_interface(&mockJeodIntegratorInterface);
        dynInst->set_default_integ_group(&dynIntegrationGroup);

        delete dynInst;

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Destructor test with JEOD-allocated params
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest * dynInst = new DynManagerTest();
        DynamicsIntegrationGroup * dynIntegrationGroup = JEOD_ALLOC_CLASS_OBJECT(DynamicsIntegrationGroup, ());

        MockSinglePointEphemeris * mockSinglePointEphemeris = JEOD_ALLOC_CLASS_OBJECT(MockSinglePointEphemeris, ());
        dynInst->set_simple_ephemeris(mockSinglePointEphemeris);

        MockJeodIntegratorInterface * mockJeodIntegratorInterface = JEOD_ALLOC_CLASS_OBJECT(MockJeodIntegratorInterface,
                                                                                            ());
        dynInst->set_integ_interface(mockJeodIntegratorInterface);
        dynInst->set_default_integ_group(dynIntegrationGroup);
        Mock::VerifyAndClear(&mockMessageHandler);

        delete dynInst;

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, statelessCalls)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    DynManagerTest staticInst;
    MockIntegratorConstructor mockIntegConstructor;
    MockJeodIntegrationTime mockJeodIntegTime;
    MockJeodIntegratorInterface mockJeodIntegInterface;
    DynamicsIntegrationGroup dynIntegrationGroup(staticInst,
                                                 mockIntegConstructor,
                                                 mockJeodIntegInterface,
                                                 mockJeodIntegTime);
    Mock::VerifyAndClear(&mockMessageHandler);

    staticInst.reset_integrators(dynIntegrationGroup);
    staticInst.shutdown();

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, get_dyn_bodies)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManager staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        auto dynBodiesList = staticInst.get_dyn_bodies();
        EXPECT_EQ(0, dynBodiesList.size());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }
}

TEST(DynManager, timestamp)
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
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_EQ(0.0, staticInst.timestamp());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, name)
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
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_EQ("DynManager", staticInst.name());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, set_gravity_manager)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Attempt to set gravity manager after already set. Report error and change nothing.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_gravity_manager(gravManager);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::singleton_error, _, _))
            .Times(1);
        staticInst.DynManager::set_gravity_manager(gravManager);
        EXPECT_EQ(&gravManager, staticInst.get_gravity_manager());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Attempt to set gravity manager after initialized. Report error and change nothing.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_initialized(true);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error,
                                    _,
                                    _,
                                    _,
                                    DynManagerMessages::inconsistent_setup,
                                    HasSubstr("initialize_simulation"),
                                    _))
            .Times(1);
        staticInst.DynManager::set_gravity_manager(gravManager);
        EXPECT_EQ(nullptr, staticInst.get_gravity_manager());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Attempt to set gravity manager when gravity is configured off. Report error and change nothing.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.gravity_off = true;

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error,
                                    _,
                                    _,
                                    _,
                                    DynManagerMessages::inconsistent_setup,
                                    HasSubstr("gravity_off"),
                                    _))
            .Times(1);
        staticInst.DynManager::set_gravity_manager(gravManager);
        EXPECT_EQ(nullptr, staticInst.get_gravity_manager());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Nominal set
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.DynManager::set_gravity_manager(gravManager);
        EXPECT_EQ(&gravManager, staticInst.get_gravity_manager());
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, add_body_action)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Add body action to uninitialized DynManager
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);
        BodyAction bodyAction;

        {
            // Before
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(0, bodyActionList.size());
        }

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.add_body_action(bodyAction);
        Mock::VerifyAndClear(&mockMessageHandler);

        {
            // After, successful addition to list
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(1, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
            // Empty identified implies uninitialized BodyAction
            EXPECT_EQ(std::string(""), bodyAction.get_identifier());
        }

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Attempt to add duplicate bodyaction. Report error and change nothing.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);
        BodyAction bodyAction;

        std::list<BodyAction *> bodyActionListInput = {&bodyAction};
        staticInst.set_body_actions(bodyActionListInput);

        {
            // Before
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(1, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
            // Empty identified implies uninitialized BodyAction
            EXPECT_EQ(std::string(""), bodyAction.get_identifier());
        }

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        staticInst.add_body_action(bodyAction);
        Mock::VerifyAndClear(&mockMessageHandler);

        {
            // After, should be unchanged with no addition
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(1, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
            // Empty identified implies uninitialized BodyAction
            EXPECT_EQ(std::string(""), bodyAction.get_identifier());
        }

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Nominal add. Expect errors from call to BodyAction::initialize but is ignored for this test.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);
        BodyAction bodyAction;

        {
            // Before
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(0, bodyActionList.size());
        }

        staticInst.set_initialized(true);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::null_pointer, _, _))
            .Times(1);
        staticInst.add_body_action(bodyAction);
        Mock::VerifyAndClear(&mockMessageHandler);

        {
            // After, successful addition to list
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(1, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
            // Empty identified implies uninitialized BodyAction
            EXPECT_EQ(std::string("jeod::BodyAction.unnamed instance"), bodyAction.get_identifier());
        }

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, remove_body_action)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Add 2 bodyactions, remove action using empty string-no change,
        // remove action using unmatched name-no change,
        // remove action using matched name-remove 1 bodyaction
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);
        BodyAction bodyAction;
        MockBodyAction mockBodyAction;

        bodyAction.action_name = "Test BodyAction";
        mockBodyAction.action_name = "Test MockBodyAction";

        std::list<BodyAction *> bodyActionListInput = {&bodyAction, &mockBodyAction};
        staticInst.set_body_actions(bodyActionListInput);

        {
            // Before
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(2, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
            EXPECT_EQ(&mockBodyAction, bodyActionList.back());
        }

        staticInst.remove_body_action("");

        {
            // After, no change.
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(2, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
            EXPECT_EQ(&mockBodyAction, bodyActionList.back());
        }

        staticInst.remove_body_action("No BodyAction");

        {
            // After, no change.
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(2, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
            EXPECT_EQ(&mockBodyAction, bodyActionList.back());
        }

        EXPECT_CALL(mockBodyAction, shutdown()).Times(1);
        staticInst.remove_body_action("Test MockBodyAction");
        Mock::VerifyAndClear(&mockBodyAction);

        {
            // After, remove mock object.
            std::list<BodyAction *> bodyActionList = staticInst.get_body_actions();
            EXPECT_EQ(1, bodyActionList.size());
            EXPECT_EQ(&bodyAction, bodyActionList.front());
        }

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, reset_integrators)
{
    // This is a semi-stateless test. There are some conditions to trigger branches but, there is nothing to look at
    // afterward to know if the test worked.
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Call reset_integrators with no integ_groups or default_integ_group
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.reset_integrators();

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call reset_integrators with no integ_groups but valid default_integ_group
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockIntegratorConstructor mockIntegConstructor;
        MockJeodIntegrationTime mockJeodIntegTime;
        MockJeodIntegratorInterface mockJeodIntegInterface;
        DynamicsIntegrationGroup dynIntegrationGroup(staticInst,
                                                     mockIntegConstructor,
                                                     mockJeodIntegInterface,
                                                     mockJeodIntegTime);
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_default_integ_group(&dynIntegrationGroup);
        staticInst.reset_integrators();

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call reset_integrators with some integ_groups
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockIntegratorConstructor mockIntegConstructor;
        MockJeodIntegrationTime mockJeodIntegTime;
        MockJeodIntegratorInterface mockJeodIntegInterface;
        DynamicsIntegrationGroup dynIntegrationGroup(staticInst,
                                                     mockIntegConstructor,
                                                     mockJeodIntegInterface,
                                                     mockJeodIntegTime);
        Mock::VerifyAndClear(&mockMessageHandler);

        std::vector<DynamicsIntegrationGroup *> integGroupsListInput = {&dynIntegrationGroup, &dynIntegrationGroup};
        staticInst.set_integ_groups(integGroupsListInput);
        staticInst.reset_integrators();

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
