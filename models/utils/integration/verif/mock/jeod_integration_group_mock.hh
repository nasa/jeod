/*
 * jeod_integration_group_mock.hh
 *
 *  Created on: Dec 13, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_INTEGRATION_VERIF_MOCK_JEOD_INTEGRATION_GROUP_MOCK_HH_
#define JEOD_MODELS_UTILS_INTEGRATION_VERIF_MOCK_JEOD_INTEGRATION_GROUP_MOCK_HH_

#include "gmock/gmock.h"

#include "utils/integration/include/jeod_integration_group.hh"

class MockJeodIntegrationGroup : public jeod::JeodIntegrationGroup
{
public:
    MOCK_METHOD1(remove_integrable_object, void(er7_utils::IntegrableObject & integrable_object));
};

#endif /* JEOD_MODELS_UTILS_INTEGRATION_VERIF_MOCK_JEOD_INTEGRATION_GROUP_MOCK_HH_ */
