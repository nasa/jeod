/*
 * integrable_object_mock.hh
 *
 *  Created on: Dec 13, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRABLE_OBJECT_MOCK_HH_
#define JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRABLE_OBJECT_MOCK_HH_

#include "gmock/gmock.h"

#include "er7_utils/integration/core/include/integrable_object.hh"
#include "er7_utils/integration/core/include/time_interface.hh"

class MockIntegrableObject : public er7_utils::IntegrableObject
{
public:
    MOCK_METHOD3(create_integrators,
                 void(const er7_utils::IntegratorConstructor & generator,
                      er7_utils::IntegrationControls & controls,
                      const er7_utils::TimeInterface & time_if));

    MOCK_METHOD0(destroy_integrators, void(void));
    MOCK_METHOD0(reset_integrators, void(void));
    MOCK_METHOD2(integrate, er7_utils::IntegratorResult(double dyn_dt, unsigned int target_stage));
};

#endif /* JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRABLE_OBJECT_MOCK_HH_ */
