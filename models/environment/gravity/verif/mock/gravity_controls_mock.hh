/*
 * gravity_controls_mock.hh
 *
 *  Created on: Dec 5, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_ENVIRONMENT_GRAVITY_VERIF_MOCK_GRAVITY_CONTROLS_MOCK_HH_
#define JEOD_MODELS_ENVIRONMENT_GRAVITY_VERIF_MOCK_GRAVITY_CONTROLS_MOCK_HH_

#include "environment/gravity/include/gravity_controls.hh"
#include "environment/gravity/include/gravity_integ_frame.hh"

#include "gmock/gmock.h"

class MockGravityControls : public jeod::GravityControls
{
public:
    MOCK_METHOD6(calc_nonspherical,
                 void(const double integ_pos[3],
                      const double posn[3],
                      const jeod::GravityIntegFrame & grav_source_frame,
                      double body_grav_accel[3],
                      double dgdx[3][3],
                      double & pot));

    MOCK_METHOD1(initialize_control, void(jeod::GravityManager & grav_man));
    MOCK_METHOD1(reset_control, void(jeod::BaseDynManager & dyn_manager));
    MOCK_METHOD5(gravitation,
                 void(const double integ_pos[3],
                      unsigned int integ_frame_idx,
                      double body_grav_accel[3],
                      double dgdx[3][3],
                      double Pot[1]));

    MOCK_METHOD5(gravitation,
                 void(const jeod::RefFrame & point_of_interest,
                      unsigned int integ_frame_idx,
                      double body_grav_accel[3],
                      double dgdx[3][3],
                      double & pot));
};

#endif /* JEOD_MODELS_ENVIRONMENT_GRAVITY_VERIF_MOCK_GRAVITY_CONTROLS_MOCK_HH_ */
