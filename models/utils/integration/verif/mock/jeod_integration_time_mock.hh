/*
 * jeod_integration_time_mock.hh
 *
 *  Created on: Jan 15, 2024
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_INTEGRATION_VERIF_MOCK_JEOD_INTEGRATION_TIME_MOCK_HH_
#define JEOD_MODELS_UTILS_INTEGRATION_VERIF_MOCK_JEOD_INTEGRATION_TIME_MOCK_HH_

#include "gmock/gmock.h"

#include "utils/integration/include/jeod_integration_time.hh"

class MockJeodIntegrationTime : public jeod::JeodIntegrationTime
{
public:
    MOCK_CONST_METHOD0(get_timestamp_time, double());
    MOCK_METHOD1(update_time, void(double sim_time));
    MOCK_CONST_METHOD0(get_time_scale_factor, double());
};

#endif /* JEOD_MODELS_UTILS_INTEGRATION_VERIF_MOCK_JEOD_INTEGRATION_TIME_MOCK_HH_ */
