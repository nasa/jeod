/*
 * spherical_harmonics_gravity_controls_ut.cc
 */

#include "environment/gravity/include/gravity_messages.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_controls.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::HasSubstr;
using testing::Mock;

using namespace jeod;

class SphericalHarmonicsGravityControlsTest : public SphericalHarmonicsGravityControls
{
public:
    void check_validity() override
    {
        SphericalHarmonicsGravityControls::check_validity();
    }
};

TEST(SphericalHarmonicsGravityControls, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SphericalHarmonicsGravityControls staticInst;
    SphericalHarmonicsGravityControls * dynInst = new SphericalHarmonicsGravityControls;
    delete dynInst;
}

TEST(SphericalHarmonicsGravityControls, initialize_control) {}

TEST(SphericalHarmonicsGravityControls, add_deltacontrol) {}

TEST(SphericalHarmonicsGravityControls, get_degree) {}

TEST(SphericalHarmonicsGravityControls, get_order) {}

TEST(SphericalHarmonicsGravityControls, get_degree_order) {}

TEST(SphericalHarmonicsGravityControls, get_grad_degree) {}

TEST(SphericalHarmonicsGravityControls, get_grad_order) {}

TEST(SphericalHarmonicsGravityControls, get_grad_degree_order) {}

TEST(SphericalHarmonicsGravityControls, set_degree) {}

TEST(SphericalHarmonicsGravityControls, set_order) {}

TEST(SphericalHarmonicsGravityControls, set_degree_order) {}

TEST(SphericalHarmonicsGravityControls, set_grad_degree) {}

TEST(SphericalHarmonicsGravityControls, set_grad_order) {}

TEST(SphericalHarmonicsGravityControls, set_grad_degree_order) {}

TEST(SphericalHarmonicsGravityControls, check_validity)
{
    MockMessageHandler mockMessageHandler;

    {
        // Test null harmonics_source. Error.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, GravityMessages::invalid_object, _, _))
            .Times(1);
        staticInst.check_validity();
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test valid harmonics_source, spherical true. No action.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.harmonics_source = &source;
        staticInst.spherical = true;
        staticInst.check_validity();
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical, degree 0. Set to spherical, warn, return.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    HasSubstr("set to zero but non-spherical gravity requested"),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        staticInst.spherical = false;
        staticInst.check_validity();
        EXPECT_EQ(staticInst.spherical, true);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical, control degree bigger than source. Error and return.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    HasSubstr("is greater than max gravity field degree"),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        source.degree = 3;
        staticInst.spherical = false;
        staticInst.degree = 5;
        staticInst.check_validity();
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical, control order bigger than source. Error and return.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    HasSubstr("is greater than max gravity field order"),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        source.degree = 3;
        source.order = 3;
        staticInst.spherical = false;
        staticInst.degree = 2;
        staticInst.order = 5;
        staticInst.check_validity();
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical, control order bigger than control degree. Error and return.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    AllOf(HasSubstr("Gravity field order("),
                                          HasSubstr("is greater than gravity field degree")),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        source.degree = 3;
        source.order = 3;
        staticInst.spherical = false;
        staticInst.degree = 2;
        staticInst.order = 3;
        staticInst.check_validity();
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical with gradient. control gradient_degree bigger than control degree. Warn, set
        // gradient_degree to degree.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    HasSubstr("Setting the gradient degree to the gravity degree"),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        source.degree = 4;
        source.order = 4;
        staticInst.spherical = false;
        staticInst.degree = 2;
        staticInst.order = 2;
        staticInst.gradient = true;
        staticInst.gradient_degree = 3;
        staticInst.check_validity();
        EXPECT_EQ(staticInst.gradient_degree, staticInst.degree);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical with gradient. gradient_degree == 1. Warn, set gradient_degree to 0.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    HasSubstr("must not equal 1"),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        source.degree = 4;
        source.order = 4;
        staticInst.spherical = false;
        staticInst.degree = 2;
        staticInst.order = 2;
        staticInst.gradient = true;
        staticInst.gradient_degree = 1;
        staticInst.check_validity();
        EXPECT_EQ(staticInst.gradient_degree, 0);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical with gradient. control gradient_order bigger than control gradient_degree. Warn, set
        // gradient_order to gradient_degree.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    HasSubstr("Setting the gradient order to the gradient degree"),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        source.degree = 4;
        source.order = 4;
        staticInst.spherical = false;
        staticInst.degree = 3;
        staticInst.order = 3;
        staticInst.gradient = true;
        staticInst.gradient_degree = 2;
        staticInst.gradient_order = 3;
        staticInst.check_validity();
        EXPECT_EQ(staticInst.gradient_order, staticInst.gradient_degree);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical with gradient. control gradient_order bigger than control order. Warn, set
        // gradient_order to order.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error,
                                    _,
                                    _,
                                    _,
                                    GravityMessages::invalid_limit,
                                    HasSubstr("Setting the gradient order to the gravity order"),
                                    _))
            .Times(1);
        staticInst.harmonics_source = &source;
        source.degree = 5;
        source.order = 5;
        staticInst.spherical = false;
        staticInst.degree = 4;
        staticInst.order = 3;
        staticInst.gradient = true;
        staticInst.gradient_degree = 4;
        staticInst.gradient_order = 4;
        staticInst.check_validity();
        EXPECT_EQ(staticInst.gradient_order, staticInst.order);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // Test non-spherical with gradient. control gradient_order bigger than control order. Warn, set
        // gradient_order to order.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        SphericalHarmonicsGravityControlsTest staticInst;
        SphericalHarmonicsGravitySource source;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.harmonics_source = &source;
        source.degree = 5;
        source.order = 5;
        staticInst.spherical = false;
        staticInst.degree = 4;
        staticInst.order = 4;
        staticInst.gradient = true;
        staticInst.gradient_degree = 3;
        staticInst.gradient_order = 3;
        staticInst.check_validity();
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(SphericalHarmonicsGravityControls, update_deltacoeffs) {}

TEST(SphericalHarmonicsGravityControls, sum_deltacoeffs) {}
