/*
 * integration_controls_mock.hh
 *
 *  Created on: Jan 15, 2024
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRATION_CONTROLS_MOCK_HH_
#define JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRATION_CONTROLS_MOCK_HH_

#include "gmock/gmock.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::Return;

#include "er7_utils/integration/core/include/integration_controls.hh"
#include "er7_utils/integration/core/include/time_interface.hh"

class MockIntegrationControls : public er7_utils::IntegrationControls
{
public:
    MockIntegrationControls()
    {
        ON_CALL(*this, create_copy()).WillByDefault(Return(this));
        EXPECT_CALL(*this, create_copy()).Times(AnyNumber());
    }

    MOCK_CONST_METHOD0(create_copy, er7_utils::IntegrationControls *(void));
    MOCK_METHOD5(integrate,
                 unsigned int(double start_time,
                              double sim_dt,
                              er7_utils::TimeInterface & time_interface,
                              er7_utils::IntegratorInterface & integ_interface,
                              er7_utils::BaseIntegrationGroup & integ_group));

    static MockIntegrationControls * get_global_instance()
    {
        static MockIntegrationControls static_mockIntegrationControls;
        return &static_mockIntegrationControls;
    }
};

#endif /* JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRATION_CONTROLS_MOCK_HH_ */
