/*
 * gravitation_ut.cc
 */

#include "dyn_body_mock.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_manager/include/dyn_manager_messages.hh"
#include "dynamics_integration_group_mock.hh"
#include "memory_interface_mock.hh"
#include "message_handler_mock.hh"
#include "simulation_interface_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;
using testing::Ref;

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

    void set_default_integ_group(DynamicsIntegrationGroup * default_integ_group_in)
    {
        default_integ_group = default_integ_group_in;
    }

    void set_dyn_bodies(const std::vector<DynBody *> & dyn_bodies_in)
    {
        dyn_bodies = dyn_bodies_in;
    }
};

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
