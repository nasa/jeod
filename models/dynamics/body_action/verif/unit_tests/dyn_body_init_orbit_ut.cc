/*
 *  dyn_body_init_orbit_ut.cc
 *
 *  Created on: June 28, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
#include "dynamics/body_action/include/dyn_body_init_orbit.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
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

class DynBodyInitOrbitTest : public DynBodyInitOrbit
{
public:
    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }

    RefFrame * get_body_ref_frame()
    {
        return body_ref_frame;
    }

    RefFrame * get_reference_ref_frame()
    {
        return reference_ref_frame;
    }

    const std::string get_reference_ref_frame_name()
    {
        return reference_ref_frame_name;
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

    void set_reference_ref_frame_name(const std::string & in)
    {
        reference_ref_frame_name = in;
    }

    void rotational()
    {
        this->compute_rotational_state();
    }

    DynBody * find_dyn_body(const DynManager & dyn_manager,
                            const std::string & dyn_body_name,
                            const std::string & variable_name)
    {
        return DynBodyInitOrbit::find_dyn_body(dyn_manager, dyn_body_name, variable_name);
    }

    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return DynBodyInitOrbit::find_planet(dyn_manager, planet_name, variable_name);
    }

    BodyRefFrame * find_body_frame(DynBody & frame_container,
                                   const std::string & body_frame_identifier,
                                   const std::string & variable_name)
    {
        return DynBodyInitOrbit::find_body_frame(frame_container, body_frame_identifier, variable_name);
    }

    RefFrame * find_ref_frame(const DynManager & dyn_manager,
                              const std::string & body_frame_identifier,
                              const std::string & variable_name)
    {
        return DynBodyInitOrbit::find_ref_frame(dyn_manager, body_frame_identifier, variable_name);
    }

    void apply_user_inputs()
    {
        DynBodyInitOrbit::apply_user_inputs();
    }

    void compute_rotational_state()
    {
        DynBodyInitOrbit::compute_rotational_state();
    }

    void compute_translational_state()
    {
        DynBodyInitOrbit::compute_translational_state();
    }

    void set_planet(Planet * in)
    {
        planet = in;
    }

    void set_orbit_frame(EphemerisRefFrame * in)
    {
        orbit_frame = in;
    }
};

TEST(DynBodyInitOrbit, create)
{
    DynBodyInitOrbit staticInst;
    DynBodyInitOrbitTest * dynInst;
    dynInst = new DynBodyInitOrbitTest();
    DynBodyInitOrbit * dynInst2 = new DynBodyInitOrbit();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_body_ref_frame());
    EXPECT_EQ(nullptr, dynInst->get_reference_ref_frame());

    delete dynInst2;
    delete dynInst;
}

TEST(DynBodyInitOrbit, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    GravitySource gravitySource;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Invalid Set
        Planet planet;
        DynBodyInitOrbitTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&planet.inertial);
        dynInst.set = jeod::DynBodyInitOrbit::InvalidSet;

        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::illegal_value, _, _))
            .Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Valid set, null grav_source
        Planet planet;
        DynBodyInitOrbitTest dynInst;
        dynInst.planet_name = "planet";
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&planet.inertial);
        dynInst.set = jeod::DynBodyInitOrbit::SmaEccIncAscnodeArgperTimeperi;

        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::invalid_object, _, _))
            .Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Valid set, valid grav_source, orbit_frame not found
        Planet planet;
        planet.grav_source = &gravitySource;
        DynBodyInitOrbitTest dynInst;
        dynInst.planet_name = "planet";
        dynInst.orbit_frame_name = "planet.inertial";
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&planet.inertial);
        dynInst.set = jeod::DynBodyInitOrbit::SmaEccIncAscnodeArgperTimeperi;

        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(nullptr));

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::invalid_name, _, _))
            .Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Valid set, valid grav_source, ref_frame found but is not an orbit_frame
        Planet planet;
        planet.grav_source = &gravitySource;
        DynBodyInitOrbitTest dynInst;
        dynInst.planet_name = "planet";
        dynInst.orbit_frame_name = "planet.inertial";
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&planet.inertial);
        dynInst.set = jeod::DynBodyInitOrbit::SmaEccIncAscnodeArgperTimeperi;

        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&mockDynBody.composite_body));

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::invalid_object, _, _))
            .Times(1);
        dynInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Valid set, valid grav_source, ref_frame found but is not an orbit_frame
        Planet planet;
        planet.grav_source = &gravitySource;
        DynBodyInitOrbitTest dynInst;
        dynInst.planet_name = "planet";
        dynInst.orbit_frame_name = "planet.inertial";
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_reference_ref_frame(&planet.pfix);
        dynInst.set = jeod::DynBodyInitOrbit::SmaEccIncAscnodeArgperTimeperi;

        EXPECT_CALL(mockDynManager, is_dyn_body_registered(_)).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockDynManager, subscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&planet.inertial));

        dynInst.initialize(mockDynManager);
        EXPECT_EQ(dynInst.get_reference_ref_frame(), &planet.inertial);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(DynBodyInitOrbit, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.composite_body.set_owner(&mockDynBody);
    Planet planet;
    planet.inertial.add_child(mockDynBody.composite_body);
    planet.inertial.add_child(planet.pfix);
    GravitySource gravitySource;
    gravitySource.mu = 1.0;
    planet.grav_source = &gravitySource;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // invalid set
        DynBodyInitOrbitTest dynInst;
        dynInst.set = jeod::DynBodyInitOrbit::InvalidSet;
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::illegal_value, _, _))
            .Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // SmaEccIncAscnodeArgperTimeperi set, orbit_frame == planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_major_axis = 1.0;
        dynInst.set = jeod::DynBodyInitOrbit::SmaEccIncAscnodeArgperTimeperi;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.inertial);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // SmaEccIncAscnodeArgperManom set, orbit_frame != planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_major_axis = 1.0;
        dynInst.set = jeod::DynBodyInitOrbit::SmaEccIncAscnodeArgperManom;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.pfix);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // SlrEccIncAscnodeArgperTanom set, orbit_frame == planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_latus_rectum = 1.0;
        dynInst.set = jeod::DynBodyInitOrbit::SlrEccIncAscnodeArgperTanom;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.inertial);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // IncAscnodeAltperAltapoArgperTanom set, orbit_frame == planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_major_axis = 1.0;
        dynInst.set = jeod::DynBodyInitOrbit::IncAscnodeAltperAltapoArgperTanom;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.inertial);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // IncAscnodeAltperAltapoArgperTimeperi set, orbit_frame == planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_major_axis = 1.0;
        dynInst.set = jeod::DynBodyInitOrbit::IncAscnodeAltperAltapoArgperTimeperi;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.inertial);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // SmaIncAscnodeArglatRadRadvel set, orbit_frame == planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_major_axis = 1.0;
        dynInst.orb_radius = 1.0;
        dynInst.set = jeod::DynBodyInitOrbit::SmaIncAscnodeArglatRadRadvel;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.inertial);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        EXPECT_EQ(dynInst.true_anomaly, 0.0);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // SmaIncAscnodeArglatRadRadvel set, orbit_frame == planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_major_axis = 3.0;
        dynInst.orb_radius = 0.5;
        dynInst.radial_vel = 0.7;
        dynInst.set = jeod::DynBodyInitOrbit::SmaIncAscnodeArglatRadRadvel;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.inertial);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        EXPECT_GT(dynInst.true_anomaly, 0.0);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // SmaEccIncAscnodeArgperTanom set, orbit_frame == planet.inertial
        DynBodyInitOrbitTest dynInst;
        dynInst.semi_major_axis = 1.0;
        dynInst.set = jeod::DynBodyInitOrbit::SmaEccIncAscnodeArgperTanom;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_body_ref_frame(&mockDynBody.composite_body);
        dynInst.set_planet(&planet);
        dynInst.set_orbit_frame(&planet.inertial);
        dynInst.set_reference_ref_frame(&planet.inertial);
        EXPECT_CALL(mockDynBody, get_integ_frame()).Times(1).WillOnce(Return(&planet.inertial));
        EXPECT_CALL(mockDynManager, unsubscribe_to_frame(_)).Times(1);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockDynManager);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
