/*
 * dyn_body_ut.cc
 *
 *  Created on: Dec 5, 2023
 *      Author: tbrain
 */

#include "dyn_manager_mock.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/gravity/include/gravity_manager.hh"
#include "gravity_controls_mock.hh"
#include "integrable_object_mock.hh"
#include "jeod_integration_group_mock.hh"
#include "memory_interface_mock.hh"
#include "message_handler_mock.hh"
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

class DynBodyTest : public DynBody
{
public:
    BodyRefFrame * get_attitude_source()
    {
        return attitude_source;
    }

    void add_to_dyn_children(DynBody * child_in)
    {
        dyn_children.push_back(child_in);
    }

    BaseDynManager *& get_dyn_manager()
    {
        return dyn_manager;
    }

    void set_dyn_manager(BaseDynManager * dyn_manager_in)
    {
        dyn_manager = dyn_manager_in;
    }

    void set_dyn_parent(DynBody * dyn_parent_in)
    {
        dyn_parent = dyn_parent_in;
    }

    BodyRefFrame * get_integrated_frame()
    {
        return integrated_frame;
    }

    void add_to_mass_children(MassBody * mass_in)
    {
        mass_children.push_back(mass_in);
    }

    BodyRefFrame * get_position_source()
    {
        return position_source;
    }

    BodyRefFrame * get_rate_source()
    {
        return rate_source;
    }

    const JeodIntegrationTime * get_time_manager()
    {
        return time_manager;
    }

    void add_to_vehicle_points(BodyRefFrame * vehicle_point_in)
    {
        vehicle_points.push_back(vehicle_point_in);
    }

    BodyRefFrame * get_velocity_source()
    {
        return velocity_source;
    }
};

TEST(DynBody, create)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    Mock::VerifyAndClear(&mockMessageHandler);

    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        DynBodyTest * dynInst;
        dynInst = new DynBodyTest();
        DynBody * dynInst2 = new DynBody();
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_TRUE(dynInst->autoupdate_vehicle_points);
        EXPECT_FALSE(dynInst->compute_point_derivative);
        EXPECT_EQ(&(dynInst->mass.name), &(dynInst->name));
        EXPECT_EQ(GeneralizedSecondOrderODETechnique::LieGroup, dynInst->rotation_integration);
        EXPECT_FALSE(dynInst->rotational_dynamics);
        EXPECT_FALSE(dynInst->three_dof);
        EXPECT_FALSE(dynInst->translational_dynamics);
        EXPECT_EQ(nullptr, dynInst->get_attitude_source());
        EXPECT_EQ(nullptr, dynInst->get_dyn_manager());
        EXPECT_EQ(nullptr, dynInst->get_parent_body());
        EXPECT_EQ(RefFrameItems::No_Items, dynInst->get_initialized_states().get());
        EXPECT_EQ(nullptr, dynInst->get_integ_frame());
        EXPECT_EQ(&(dynInst->composite_body), dynInst->get_integrated_frame());
        EXPECT_EQ(nullptr, dynInst->get_position_source());
        EXPECT_EQ(nullptr, dynInst->get_rate_source());
        EXPECT_EQ(nullptr, dynInst->get_time_manager());
        EXPECT_EQ(nullptr, dynInst->get_velocity_source());

        EXPECT_EQ(dynInst, dynInst->mass.dyn_owner);
        EXPECT_TRUE(dynInst->mass.compute_inverse_inertia);
        EXPECT_EQ(&(dynInst->mass.core_properties), dynInst->core_body.mass_point);
        EXPECT_EQ(&(dynInst->mass.composite_properties), dynInst->composite_body.mass_point);
        EXPECT_EQ(&(dynInst->mass.structure_point), dynInst->structure.mass_point);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, HasSubstr("master memory manager"), _))
            .Times(AnyNumber());
        delete dynInst2;
        delete dynInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Destructor test, dyn_manager assigned and all parameters of interest populated.
        // vehicle_points populated by standard allocated BodyRefFrame
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        BodyRefFrame bodyRefFrame;
        DynBody parentBody;
        DynBody childBody;
        MassBody massBody;
        DynBodyTest * dynInst = new DynBodyTest;
        Mock::VerifyAndClear(&mockMessageHandler);

        dynInst->set_dyn_parent(&parentBody);
        dynInst->add_to_dyn_children(&childBody);
        dynInst->add_to_mass_children(&massBody);
        dynInst->set_dyn_manager(&mockDynManager);
        dynInst->add_to_vehicle_points(&bodyRefFrame);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Notice, _, _, _, _, HasSubstr("No detachment was performed"), _))
            .Times(2);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Notice, _, _, _, _, HasSubstr("detach sub-body MassBody"), _))
            .Times(1);
        EXPECT_CALL(mockDynManager, remove_ref_frame(Ref(dynInst->core_body))).Times(1);
        EXPECT_CALL(mockDynManager, remove_ref_frame(Ref(dynInst->composite_body))).Times(1);
        EXPECT_CALL(mockDynManager, remove_ref_frame(Ref(dynInst->structure))).Times(1);
        EXPECT_CALL(mockDynManager, remove_ref_frame(Ref(bodyRefFrame))).Times(1);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, HasSubstr("master memory manager"), _))
            .Times(AnyNumber());
        delete dynInst;

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Destructor test, null dyn_manager. vehicle_points populated by JEOD-allocated BodyRefFrame
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

        DynBodyTest * dynInst = new DynBodyTest;
        BodyRefFrame * bodyRefFrame = JEOD_ALLOC_CLASS_OBJECT(BodyRefFrame, ());
        Mock::VerifyAndClear(&mockMessageHandler);

        dynInst->add_to_vehicle_points(bodyRefFrame);
        dynInst->add_to_vehicle_points(nullptr);

        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(0);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, HasSubstr("master memory manager"), _))
            .Times(AnyNumber());
        delete dynInst;

        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, statelessCalls)
{
    // Special test for coverage only. Need to call something that doesn't have a state of variable to check?
    // Add it here.
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    GravityManager gravityManager;
    DynBodyTest staticInst;
    Mock::VerifyAndClear(&mockMessageHandler);

    staticInst.set_dyn_manager(&mockDynManager);
    staticInst.initialize_controls(gravityManager);
    staticInst.reset_controls();
    staticInst.sort_controls();

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(AnyNumber());
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, is_root_body)
{
    MockMessageHandler mockMessageHandler;

    {
        // No parent
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_TRUE(staticInst.is_root_body());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // With parent
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody parentBody;
        DynBodyTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_dyn_parent(&parentBody);
        EXPECT_FALSE(staticInst.is_root_body());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, get_root_body)
{
    MockMessageHandler mockMessageHandler;

    {
        // No parent, returns self
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_EQ(&staticInst, staticInst.get_root_body());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // With parent
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody parentBody;
        DynBodyTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_dyn_parent(&parentBody);
        EXPECT_EQ(&parentBody, staticInst.get_root_body());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, add_control)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    DynBody staticInst;
    MockGravityControls gravityControl;
    Mock::VerifyAndClear(&mockMessageHandler);

    EXPECT_EQ(0, staticInst.grav_interaction.grav_controls.size());
    staticInst.add_control(&gravityControl);
    EXPECT_EQ(1, staticInst.grav_interaction.grav_controls.size());

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, get_dynamics_integration_group)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // No integ group, returns null
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_EQ(nullptr, staticInst.get_dynamics_integration_group());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // With integ group but is of the wrong type, report failure and return null
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        JeodIntegrationGroup jeodIntegrationGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_integration_group(jeodIntegrationGroup);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_EQ(nullptr, staticInst.get_dynamics_integration_group());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // With integ group of the correct type
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        DynamicsIntegrationGroup dynIntegrationGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_integration_group(dynIntegrationGroup);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        EXPECT_EQ(&dynIntegrationGroup, staticInst.get_dynamics_integration_group());

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, add_integrable_object)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Nominally add two objects, Attempt to add one again and produce warning without modifying container
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        MockIntegrableObject integObj;
        MockIntegrableObject integObj2;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.add_integrable_object(integObj);
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            EXPECT_EQ(1, res.size());
            EXPECT_EQ(&integObj, res[0]);
        }
        staticInst.add_integrable_object(integObj2);
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            EXPECT_EQ(2, res.size());
            EXPECT_EQ(&integObj, res[0]);
            EXPECT_EQ(&integObj2, res[1]);
        }
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(1);
        staticInst.add_integrable_object(integObj);
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            EXPECT_EQ(2, res.size());
            EXPECT_EQ(&integObj, res[0]);
            EXPECT_EQ(&integObj2, res[1]);
        }
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }
    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, remove_integrable_object)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Nominally add two objects, remove one object nominally. Attempt to remove the same one and
        // produce warning without modifying container
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        MockIntegrableObject integObj;
        MockIntegrableObject integObj2;
        staticInst.add_integrable_object(integObj);
        staticInst.add_integrable_object(integObj2);
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            ASSERT_EQ(2, res.size());
        }
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.remove_integrable_object(integObj2);
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            EXPECT_EQ(1, res.size());
            EXPECT_EQ(&integObj, res[0]);
        }
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(1);
        staticInst.remove_integrable_object(integObj2);
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            EXPECT_EQ(1, res.size());
            EXPECT_EQ(&integObj, res[0]);
        }
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, clear_integrable_objects)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        MockIntegrableObject integObj;
        MockIntegrableObject integObj2;
        staticInst.add_integrable_object(integObj);
        staticInst.add_integrable_object(integObj2);
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            ASSERT_EQ(2, res.size());
        }
        staticInst.clear_integrable_objects();
        {
            JeodPointerVector<er7_utils::IntegrableObject>::type res = staticInst.get_integrable_objects();
            EXPECT_EQ(0, res.size());
        }
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBody, migrate_integrable_objects)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test null integ group, errors
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        MockIntegrableObject integObj;
        MockIntegrableObject integObj2;
        staticInst.add_integrable_object(integObj);
        staticInst.add_integrable_object(integObj2);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);
        staticInst.migrate_integrable_objects();

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Nominal migration
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        MockJeodIntegrationGroup mockJeodIntegrationGroup;
        Mock::VerifyAndClear(&mockMessageHandler);

        MockIntegrableObject integObj;
        MockIntegrableObject integObj2;
        staticInst.add_integrable_object(integObj);
        staticInst.add_integrable_object(integObj2);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(0);
        staticInst.set_integration_group(mockJeodIntegrationGroup);

        EXPECT_CALL(mockJeodIntegrationGroup, remove_integrable_object(_)).Times(2);
        staticInst.migrate_integrable_objects();
        Mock::VerifyAndClear(&mockJeodIntegrationGroup);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
