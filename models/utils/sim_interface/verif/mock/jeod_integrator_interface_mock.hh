/*
 * jeod_integrator_interface_mock.hh
 *
 *  Created on: Dec 13, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_SIM_INTERFACE_VERIF_MOCK_JEOD_INTEGRATOR_INTERFACE_MOCK_HH_
#define JEOD_MODELS_UTILS_SIM_INTERFACE_VERIF_MOCK_JEOD_INTEGRATOR_INTERFACE_MOCK_HH_

#include "gmock/gmock.h"
using testing::AnyNumber;

#include "utils/sim_interface/include/jeod_integrator_interface.hh"

class MockJeodIntegratorInterface : public jeod::JeodIntegratorInterface
{
public:
    MockJeodIntegratorInterface()
    {
        EXPECT_CALL(*this, reset_first_step_derivs_flag()).Times(AnyNumber());
    }

    MOCK_CONST_METHOD1(interpret_integration_type, er7_utils::Integration::Technique(int));
    MOCK_METHOD0(get_integrator, JEOD_SIM_INTEGRATOR_POINTER_TYPE());
    MOCK_CONST_METHOD0(get_dt, double());
    MOCK_CONST_METHOD0(get_first_step_derivs_flag, bool());
    MOCK_METHOD1(set_first_step_derivs_flag, void(bool value));
    MOCK_METHOD0(reset_first_step_derivs_flag, void());
    MOCK_METHOD0(restore_first_step_derivs_flag, void());
    MOCK_METHOD1(set_step_number, void(unsigned int stepno));
    MOCK_METHOD1(set_time, void(double time));
};

#endif /* JEOD_MODELS_UTILS_SIM_INTERFACE_VERIF_MOCK_JEOD_INTEGRATOR_INTERFACE_MOCK_HH_ */
