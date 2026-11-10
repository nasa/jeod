/*
 * gravity_controls_ut.cc
 */

#include <initializer_list>

#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/gravity/include/gravity_controls.hh"
#include "environment/gravity/include/gravity_integ_frame.hh"
#include "environment/gravity/include/gravity_manager.hh"
#include "environment/gravity/include/gravity_messages.hh"
#include "environment/gravity/include/gravity_source.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;
using testing::StrEq;

using namespace jeod;

namespace
{

class GravityControlsTest : public GravityControls
{
public:
    using GravityControls::calc_spherical;

protected:
    void calc_nonspherical(
        const double[3], const double[3], const GravityIntegFrame &, double[3], double[3][3], double &) override
    {
    }
};

struct SphericalGravityTestData
{
    EphemerisRefFrame source_frame;
    EphemerisRefFrame integ_frame;
    EphemerisRefFrame earth_frame;
    EphemerisRefFrame moon_frame;
    GravitySource source;
    GravitySource earth;
    GravitySource moon;
    GravityManager manager;
    GravityControlsTest control;
    GravityIntegFrame frame;

    SphericalGravityTestData()
    {
        source_frame.set_name("Sun.inertial");
        integ_frame.set_name("Test.inertial");
        earth_frame.set_name("Earth.inertial");
        moon_frame.set_name("Moon.inertial");
        source_frame.add_child(integ_frame);
        integ_frame.add_child(earth_frame);
        integ_frame.add_child(moon_frame);

        source.name = "Sun";
        source.mu = 125.0;
        source.inertial = &source_frame;
        earth.name = "Earth";
        earth.mu = 3.0;
        earth.inertial = &earth_frame;
        moon.name = "Moon";
        moon.mu = 1.0;
        moon.inertial = &moon_frame;
        manager.add_grav_source(earth);
        manager.add_grav_source(moon);

        control.body = &source;
        control.initialize_control(manager);
        frame.ref_frame = &integ_frame;
    }
};

void expect_vector_near(const double actual[3], const double expected[3])
{
    constexpr double tolerance = 1.0e-12;
    for(int ii = 0; ii < 3; ++ii)
    {
        SCOPED_TRACE(testing::Message() << "component = " << ii);
        EXPECT_NEAR(actual[ii], expected[ii], tolerance);
    }
}

} // namespace

TEST(GravityControls, initialize_control) {}

TEST(GravityControls, reset_control) {}

TEST(GravityControls, gravitation) {}

TEST(GravityControls, calc_spherical)
{
    {
        SCOPED_TRACE("Direct gravity, accumulation, potential, and gradient");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalGravityTestData data;
        ASSERT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        // The frame name alone must not enable the third-body correction.
        data.integ_frame.set_name("EMBary.inertial");
        data.frame.is_third_body = false;
        const double integ_pos[3] = {3.0, 4.0, 0.0};
        const double posn[3] = {3.0, 4.0, 0.0};
        const double expected_accel[3] = {-2.0, -2.0, 3.0};
        const double initial_gradient[3][3] = {
            {1.0, 2.0, 3.0},
            {2.0, 4.0, 5.0},
            {3.0, 5.0, 6.0}
        };
        const double spherical_gradient[3][3] = {
            {0.08, 1.44,  0.0},
            {1.44, 0.92,  0.0},
            { 0.0,  0.0, -1.0}
        };

        for(bool compute_gradient : {false, true})
        {
            SCOPED_TRACE(testing::Message() << "gradient = " << compute_gradient);
            data.control.gradient = compute_gradient;
            double accel[3] = {1.0, 2.0, 3.0};
            double dgdx[3][3] = {
                {1.0, 2.0, 3.0},
                {2.0, 4.0, 5.0},
                {3.0, 5.0, 6.0}
            };
            double pot = 7.0;

            data.control.calc_spherical(integ_pos, posn, data.frame, accel, dgdx, pot);

            expect_vector_near(accel, expected_accel);
            EXPECT_NEAR(pot, 32.0, 1.0e-12);
            for(int ii = 0; ii < 3; ++ii)
            {
                for(int jj = 0; jj < 3; ++jj)
                {
                    SCOPED_TRACE(testing::Message() << "row = " << ii << ", column = " << jj);
                    const double expected = initial_gradient[ii][jj] +
                                            (compute_gradient ? spherical_gradient[ii][jj] : 0.0);
                    EXPECT_NEAR(dgdx[ii][jj], expected, 1.0e-12);
                }
            }
        }

        EXPECT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        SCOPED_TRACE("Ordinary third-body gravity and Battin fallback");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalGravityTestData data;
        ASSERT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        data.frame.is_third_body = true;
        data.frame.pos[0] = 10.0;

        struct Case
        {
            const char * description;
            double integ_pos[3];
            double posn[3];
            double expected_accel[3];
        };

        const Case cases[] = {
            {   "Battin branch: q = 0.69",
             {2.0, 5.0, 0.0},
             {12.0, 5.0, 0.0},
             {1.25 - 1500.0 / 2197.0, -625.0 / 2197.0, 0.0}                                               },
            {"Fallback branch: q = -0.75", {-7.0, 4.0, 0.0}, {3.0, 4.0, 0.0},           {-1.75, -4.0, 0.0}}
        };

        for(const auto & test_case : cases)
        {
            SCOPED_TRACE(test_case.description);
            for(bool battin : {false, true})
            {
                SCOPED_TRACE(testing::Message() << "battin_method = " << battin);
                data.control.battin_method = battin;
                double accel[3] = {};
                double dgdx[3][3] = {};
                double pot = 0.0;

                data.control.calc_spherical(test_case.integ_pos, test_case.posn, data.frame, accel, dgdx, pot);

                expect_vector_near(accel, test_case.expected_accel);
            }
        }

        EXPECT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        SCOPED_TRACE("Earth-Moon barycenter uses mu-weighted constituent accelerations");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalGravityTestData data;
        ASSERT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        data.integ_frame.set_name("EMBary.inertial");
        data.frame.is_third_body = true;

        // Source -> EMB = (2, 3, 0).
        // Source -> Earth = (3, 4, 0); source -> Moon = (-1, 0, 0).
        // With mu_E = 3 and mu_M = 1, EMB is their barycenter.
        data.integ_frame.state.trans.position[0] = 2.0;
        data.integ_frame.state.trans.position[1] = 3.0;
        data.earth_frame.state.trans.position[0] = 1.0;
        data.earth_frame.state.trans.position[1] = 1.0;
        data.moon_frame.state.trans.position[0] = -3.0;
        data.moon_frame.state.trans.position[1] = -3.0;
        data.frame.pos[0] = 2.0;
        data.frame.pos[1] = 3.0;

        const double integ_pos[3] = {-2.0, -3.0, 5.0};
        const double posn[3] = {0.0, 0.0, 5.0};

        // Vehicle acceleration = (0, 0, -5).
        // Earth acceleration = (-3, -4, 0); Moon acceleration = (125, 0, 0).
        // EMB acceleration = 3/4 * Earth + 1/4 * Moon = (29, -3, 0).
        const double expected_accel[3] = {-29.0, 3.0, -5.0};

        for(bool battin : {false, true})
        {
            SCOPED_TRACE(testing::Message() << "battin_method = " << battin);
            data.control.battin_method = battin;
            data.control.gradient = true;
            double accel[3] = {};
            double dgdx[3][3] = {};
            double pot = 0.0;

            data.control.calc_spherical(integ_pos, posn, data.frame, accel, dgdx, pot);

            expect_vector_near(accel, expected_accel);
            EXPECT_NEAR(pot, 25.0, 1.0e-12);
            const double expected_gradient[3][3] = {
                {-1.0,  0.0, 0.0},
                { 0.0, -1.0, 0.0},
                { 0.0,  0.0, 2.0}
            };
            for(int ii = 0; ii < 3; ++ii)
            {
                for(int jj = 0; jj < 3; ++jj)
                {
                    SCOPED_TRACE(testing::Message() << "row = " << ii << ", column = " << jj);
                    EXPECT_NEAR(dgdx[ii][jj], expected_gradient[ii][jj], 1.0e-12);
                }
            }
        }

        EXPECT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        SCOPED_TRACE("Invalid Earth-Moon barycenter configuration");

        struct Case
        {
            const char * description;
            void (*configure)(SphericalGravityTestData &);
        };

        const Case cases[] = {
            {                  "Missing Earth source",
             [](SphericalGravityTestData & data)
             {
             data.earth.name = "OtherEarth";
             }},
            {                   "Missing Moon source",
             [](SphericalGravityTestData & data)
             {
             data.moon.name = "OtherMoon";
             }},
            {          "Missing Earth inertial frame",
             [](SphericalGravityTestData & data)
             {
             data.earth.inertial = nullptr;
             }},
            {           "Missing Moon inertial frame",
             [](SphericalGravityTestData & data)
             {
             data.moon.inertial = nullptr;
             }},
            {    "Zero total gravitational parameter",
             [](SphericalGravityTestData & data)
             {
             data.earth.mu = 0.0;
             data.moon.mu = 0.0;
             }},
            {"Negative total gravitational parameter",
             [](SphericalGravityTestData & data)
             {
             data.earth.mu = -2.0;
             data.moon.mu = 1.0;
             }}
        };

        for(const Case & test_case : cases)
        {
            SCOPED_TRACE(test_case.description);
            MockMessageHandler mockMessageHandler;
            EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
            SphericalGravityTestData data;
            ASSERT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));

            data.integ_frame.set_name("EMBary.inertial");
            data.frame.is_third_body = true;
            data.frame.pos[0] = 10.0;
            data.control.gradient = true;

            test_case.configure(data);

            EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
            EXPECT_CALL(mockMessageHandler,
                        process_message(MessageHandler::Failure, _, _, _, StrEq(GravityMessages::missing_entry), _, _))
                .Times(1);

            const double integ_pos[3] = {2.0, 5.0, 0.0};
            const double posn[3] = {12.0, 5.0, 0.0};
            double accel[3] = {1.0, 2.0, 3.0};
            double dgdx[3][3] = {};
            double pot = 7.0;

            data.control.calc_spherical(integ_pos, posn, data.frame, accel, dgdx, pot);

            // A mocked Failure returns; verify the function then exits cleanly.
            const double expected_accel[3] = {1.0, 2.0, 3.0};
            expect_vector_near(accel, expected_accel);
            EXPECT_DOUBLE_EQ(pot, 7.0);
            for(const auto & row : dgdx)
            {
                for(double value : row)
                {
                    EXPECT_DOUBLE_EQ(value, 0.0);
                }
            }

            EXPECT_TRUE(Mock::VerifyAndClearExpectations(&mockMessageHandler));
            EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        }
    }
}

TEST(GravityControls, calc_relativistic) {}
