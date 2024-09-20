/*
 * dyn_manager_ut.cc
 *
 *  Created on: Dec 13, 2023
 *      Author: tbrain
 */

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

class DynManagerTest : public DynManager
{
public:
    void set_initialized(bool initialized_in)
    {
        initialized = initialized_in;
    }

    GravityManager * get_gravity_manager()
    {
        return gravity_manager;
    }

    void set_gravity_manager(GravityManager & gravity_manager_in)
    {
        gravity_manager = &gravity_manager_in;
    }

    er7_utils::IntegratorConstructor * get_integ_constructor()
    {
        return integ_constructor;
    }

    JeodIntegratorInterface * get_integ_interface()
    {
        return integ_interface;
    }

    void set_integ_interface(JeodIntegratorInterface * integ_interface_in)
    {
        integ_interface = integ_interface_in;
    }

    DynamicsIntegrationGroup * get_default_integ_group()
    {
        return default_integ_group;
    }

    void set_default_integ_group(DynamicsIntegrationGroup * default_integ_group_in)
    {
        default_integ_group = default_integ_group_in;
    }

    SinglePointEphemeris * get_simple_ephemeris()
    {
        return simple_ephemeris;
    }

    void set_simple_ephemeris(SinglePointEphemeris * simple_ephemeris_in)
    {
        simple_ephemeris = simple_ephemeris_in;
    }

    void set_dyn_bodies(const std::vector<DynBody *> & dyn_bodies_in)
    {
        dyn_bodies = dyn_bodies_in;
    }

    std::vector<MassBody *> get_mass_bodies()
    {
        return mass_bodies;
    }

    void set_mass_bodies(const std::vector<MassBody *> & mass_bodies_in)
    {
        mass_bodies = mass_bodies_in;
    }

    std::vector<DynamicsIntegrationGroup *> get_integ_groups()
    {
        return integ_groups;
    }

    void set_integ_groups(const std::vector<DynamicsIntegrationGroup *> & integ_groups_in)
    {
        integ_groups = integ_groups_in;
    }

    std::list<BodyAction *> get_body_actions()
    {
        return body_actions;
    }

    void set_body_actions(const std::list<BodyAction *> & body_actions_in)
    {
        body_actions = body_actions_in;
    }
};

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

TEST(DynManager, initialize_gravity_controls)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Call method with no gravity_manager. Report error and turn off gravity.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.gravity_off = false;

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(1);
        staticInst.initialize_gravity_controls();

        EXPECT_TRUE(staticInst.gravity_off);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Nominal call. Check that initialize_controls is called for each dynbody
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_gravity_manager(gravManager);
        std::vector<DynBody *> dynBodiesListInput = {&mockDynBody, &mockDynBody};
        staticInst.set_dyn_bodies(dynBodiesListInput);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(mockDynBody, initialize_controls(Ref(gravManager))).Times(2);
        staticInst.initialize_gravity_controls();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, reset_gravity_controls)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Call method with no gravity_manager and gravity still on. Report error and turn off gravity.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_FALSE(staticInst.gravity_off);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(1);
        staticInst.reset_gravity_controls();
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_TRUE(staticInst.gravity_off);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call method with no gravity_manager but gravity is off. Do nothing.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        std::vector<DynBody *> dynBodiesListInput = {&mockDynBody, &mockDynBody};
        staticInst.set_dyn_bodies(dynBodiesListInput);

        staticInst.gravity_off = true;
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(0);
        EXPECT_CALL(mockDynBody, reset_controls()).Times(0);
        staticInst.reset_gravity_controls();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        EXPECT_TRUE(staticInst.gravity_off);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Nominal. Call reset_controls for all dynbody's.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        MockDynBody mockDynBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        std::vector<DynBody *> dynBodiesListInput = {&mockDynBody, &mockDynBody};
        staticInst.set_dyn_bodies(dynBodiesListInput);

        staticInst.set_gravity_manager(gravManager);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(0);
        EXPECT_CALL(mockDynBody, reset_controls()).Times(2);
        staticInst.reset_gravity_controls();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        EXPECT_FALSE(staticInst.gravity_off);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynManager, gravitation)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Call method with no gravity_manager, gravity still on, uninitialized.
        // Report error and turn off gravity and change to initialized.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynamicsIntegrationGroup mockDynIntegGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_default_integ_group(&mockDynIntegGroup);
        EXPECT_FALSE(staticInst.gravity_off);
        EXPECT_FALSE(staticInst.is_initialized());
        EXPECT_CALL(mockDynIntegGroup, gravitation(_, _)).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(1);
        staticInst.gravitation();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynIntegGroup);
        EXPECT_TRUE(staticInst.gravity_off);
        EXPECT_TRUE(staticInst.is_initialized());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call method with no gravity_manager, gravity still on, but initialized. Do nothing
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        MockDynamicsIntegrationGroup mockDynIntegGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_default_integ_group(&mockDynIntegGroup);
        staticInst.set_initialized(true);
        EXPECT_FALSE(staticInst.gravity_off);
        EXPECT_CALL(mockDynIntegGroup, gravitation(_, _)).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(0);
        staticInst.gravitation();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynIntegGroup);
        EXPECT_FALSE(staticInst.gravity_off);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call method with a gravity_manager, gravity still on, uninitialized.
        // Report error, turn off gravity, change to initialized and clear gravity_manager pointer.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        MockDynamicsIntegrationGroup mockDynIntegGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_default_integ_group(&mockDynIntegGroup);
        staticInst.set_gravity_manager(gravManager);
        EXPECT_FALSE(staticInst.gravity_off);
        EXPECT_FALSE(staticInst.is_initialized());
        EXPECT_CALL(mockDynIntegGroup, gravitation(_, _)).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(1);
        staticInst.gravitation();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynIntegGroup);
        EXPECT_TRUE(staticInst.gravity_off);
        EXPECT_TRUE(staticInst.is_initialized());
        EXPECT_EQ(nullptr, staticInst.get_gravity_manager());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call method with a gravity_manager, gravity off, initialized. Do nothing.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        MockDynamicsIntegrationGroup mockDynIntegGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_gravity_manager(gravManager);
        staticInst.set_initialized(true);
        staticInst.set_default_integ_group(&mockDynIntegGroup);
        staticInst.gravity_off = true;
        EXPECT_CALL(mockDynIntegGroup, gravitation(_, _)).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(0);
        staticInst.gravitation();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynIntegGroup);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Call method with a gravity_manager, gravity on, initialized. Call gravitation for default integ group.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynManagerTest staticInst;
        GravityManager gravManager;
        MockDynamicsIntegrationGroup mockDynIntegGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_gravity_manager(gravManager);
        staticInst.set_initialized(true);
        staticInst.set_default_integ_group(&mockDynIntegGroup);
        EXPECT_CALL(mockDynIntegGroup, gravitation(Ref(staticInst), Ref(gravManager))).Times(1);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, DynManagerMessages::inconsistent_setup, _, _))
            .Times(0);
        staticInst.gravitation();
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynIntegGroup);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
