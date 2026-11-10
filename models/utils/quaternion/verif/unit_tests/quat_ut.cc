/*
 * quat_ut.cc
 */

#include <cmath>
#include <limits>

#include "message_handler_mock.hh"
#include "utils/quaternion/include/quat.hh"
#include "utils/quaternion/include/quat_messages.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using testing::_;
using testing::Mock;
using testing::StrEq;

using namespace jeod;

namespace
{

// Compare orientations while allowing either quaternion sign.
void expect_same_orientation(const Quaternion & actual, const Quaternion & expected)
{
    constexpr double tolerance = 1.0e-14;
    const double dot = actual.scalar * expected.scalar + actual.vector[0] * expected.vector[0] +
                       actual.vector[1] * expected.vector[1] + actual.vector[2] * expected.vector[2];
    const double sign = dot < 0.0 ? -1.0 : 1.0;

    EXPECT_NEAR(actual.norm_sq(), 1.0, tolerance);
    EXPECT_NEAR(actual.scalar, sign * expected.scalar, tolerance);
    for(int ii = 0; ii < 3; ++ii)
    {
        EXPECT_NEAR(actual.vector[ii], sign * expected.vector[ii], tolerance);
    }
}

} // namespace

TEST(Quaternion, create)
{
    Quaternion staticInst;
    Quaternion * dynInst = new Quaternion;
    delete dynInst;
}

TEST(Quaternion, compute_slerp)
{
    {
        SCOPED_TRACE("Regular interpolation");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        const Quaternion q1;
        const double end_vector[3] = {1.0, 0.0, 0.0};
        const Quaternion q2(0.0, end_vector);
        const double half_pi = std::acos(-1.0) / 2.0;
        const double fractions[] = {0.0, 0.25, 0.5, 0.75, 1.0};

        // Identity to a 180-degree rotation about x, at constant angular speed.
        for(double fraction : fractions)
        {
            SCOPED_TRACE(fraction);
            const double angle = fraction * half_pi;
            const double expected_vector[3] = {std::sin(angle), 0.0, 0.0};
            const Quaternion expected(std::cos(angle), expected_vector);
            expect_same_orientation(Quaternion::compute_slerp(q1, q2, fraction), expected);
        }
    }

    {
        SCOPED_TRACE("Identical and opposite quaternions");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        const double vector[3] = {0.5, -0.5, 0.5};
        const Quaternion q1(0.5, vector);
        Quaternion opposite(q1);
        opposite.scale(-1.0);
        const double fractions[] = {0.0, 0.25, 0.5, 0.75, 1.0};

        for(double fraction : fractions)
        {
            SCOPED_TRACE(fraction);
            expect_same_orientation(Quaternion::compute_slerp(q1, q1, fraction), q1);
            expect_same_orientation(Quaternion::compute_slerp(q1, opposite, fraction), q1);
        }
    }

    {
        SCOPED_TRACE("Shortest path");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        // These rotations are 120 degrees apart along the shortest path.
        // Their quaternion dot product is negative even though both scalars
        // are positive, exercising the shortest-path sign adjustment.
        const double q1_vector[3] = {std::sqrt(3.0) / 2.0, 0.0, 0.0};
        const double q2_vector[3] = {-std::sqrt(3.0) / 2.0, 0.0, 0.0};
        const Quaternion q1(0.5, q1_vector);
        const Quaternion q2(0.5, q2_vector);
        const double midpoint_vector[3] = {1.0, 0.0, 0.0};
        const Quaternion midpoint(0.0, midpoint_vector);

        expect_same_orientation(Quaternion::compute_slerp(q1, q2, 0.0), q1);
        expect_same_orientation(Quaternion::compute_slerp(q1, q2, 0.5), midpoint);
        expect_same_orientation(Quaternion::compute_slerp(q1, q2, 1.0), q2);
    }

    {
        SCOPED_TRACE("Nearly identical quaternions");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        // Small enough to select normalized linear interpolation, but nonzero.
        constexpr double half_angle = 1.0e-7;
        const Quaternion q1;
        const double q2_vector[3] = {0.0, std::sin(half_angle), 0.0};
        const Quaternion q2(std::cos(half_angle), q2_vector);
        const double fractions[] = {0.0, 0.25, 0.5, 0.75, 1.0};

        for(double fraction : fractions)
        {
            SCOPED_TRACE(fraction);
            const double expected_vector[3] = {0.0, std::sin(fraction * half_angle), 0.0};
            const Quaternion expected(std::cos(fraction * half_angle), expected_vector);
            expect_same_orientation(Quaternion::compute_slerp(q1, q2, fraction), expected);
        }
    }

    {
        SCOPED_TRACE("Normalizes inputs");
        MockMessageHandler mockMessageHandler;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        const Quaternion q1(-2.0);
        const double q2_vector[3] = {0.0, 0.0, 3.0};
        const Quaternion q2(0.0, q2_vector);
        const double expected_vector[3] = {0.0, 0.0, std::sqrt(0.5)};
        const Quaternion expected(std::sqrt(0.5), expected_vector);

        expect_same_orientation(Quaternion::compute_slerp(q1, q2, 0.5), expected);
        expect_same_orientation(Quaternion::compute_slerp(q1, q2, 0.0), Quaternion());
        const double unit_vector[3] = {0.0, 0.0, 1.0};
        expect_same_orientation(Quaternion::compute_slerp(q1, q2, 1.0), Quaternion(0.0, unit_vector));

        // Interpolation must leave the original components unchanged.
        EXPECT_DOUBLE_EQ(q1.scalar, -2.0);
        EXPECT_DOUBLE_EQ(q2.scalar, 0.0);
        for(int ii = 0; ii < 3; ++ii)
        {
            EXPECT_DOUBLE_EQ(q1.vector[ii], 0.0);
            EXPECT_DOUBLE_EQ(q2.vector[ii], q2_vector[ii]);
        }
    }

    {
        SCOPED_TRACE("Invalid quaternions");
        MockMessageHandler mockMessageHandler;
        const Quaternion valid;
        const double invalid_values[] = {std::numeric_limits<double>::quiet_NaN(),
                                         std::numeric_limits<double>::infinity(),
                                         -std::numeric_limits<double>::infinity(),
                                         std::numeric_limits<double>::max()};

        // Check zero, too-small, and nonfinite/overflowing norms in either input.
        // Put invalid values in each component to cover scalar and vector data.
        for(int input = 0; input < 2; ++input)
        {
            SCOPED_TRACE(input);
            for(int component = 0; component < 4; ++component)
            {
                SCOPED_TRACE(component);
                for(double value : {0.0, 1.0e-8})
                {
                    SCOPED_TRACE(value);
                    double components[4] = {};
                    components[component] = value;
                    const Quaternion invalid(components);

                    EXPECT_CALL(mockMessageHandler,
                                process_message(MessageHandler::Failure, _, _, _, StrEq(QuatMessages::undefined), _, _))
                        .Times(1);
                    if(input == 0)
                    {
                        Quaternion::compute_slerp(invalid, valid, 0.5);
                    }
                    else
                    {
                        Quaternion::compute_slerp(valid, invalid, 0.5);
                    }
                    Mock::VerifyAndClearExpectations(&mockMessageHandler);
                }

                for(double value : invalid_values)
                {
                    SCOPED_TRACE(value);
                    double components[4] = {1.0, 0.0, 0.0, 0.0};
                    components[component] = value;
                    const Quaternion invalid(components);

                    EXPECT_CALL(mockMessageHandler,
                                process_message(MessageHandler::Failure, _, _, _, StrEq(QuatMessages::undefined), _, _))
                        .Times(1);
                    if(input == 0)
                    {
                        Quaternion::compute_slerp(invalid, valid, 0.5);
                    }
                    else
                    {
                        Quaternion::compute_slerp(valid, invalid, 0.5);
                    }
                    Mock::VerifyAndClearExpectations(&mockMessageHandler);
                }
            }
        }
    }

    {
        SCOPED_TRACE("Invalid interpolation fraction");
        MockMessageHandler mockMessageHandler;
        const Quaternion q1;
        const Quaternion q2;
        const double invalid_fractions[] = {-0.1,
                                            1.1,
                                            std::numeric_limits<double>::quiet_NaN(),
                                            std::numeric_limits<double>::infinity(),
                                            -std::numeric_limits<double>::infinity()};

        for(double fraction : invalid_fractions)
        {
            SCOPED_TRACE(fraction);
            EXPECT_CALL(mockMessageHandler,
                        process_message(MessageHandler::Failure, _, _, _, StrEq(QuatMessages::undefined), _, _))
                .Times(1);
            Quaternion::compute_slerp(q1, q2, fraction);
            Mock::VerifyAndClearExpectations(&mockMessageHandler);
        }
    }
}
