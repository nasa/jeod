/*
 * integrator_constructor_mock.hh
 *
 *  Created on: Jan 15, 2024
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRATOR_CONSTRUCTOR_MOCK_HH_
#define JEOD_MODELS_UTILS_INTEGRATION_VERIF_ER7_UTILS_STUBS_MOCK_INTEGRATOR_CONSTRUCTOR_MOCK_HH_

#include "gmock/gmock.h"

#include "er7_utils/integration/core/include/integrator_constructor.hh"

class MockIntegratorConstructor : public er7_utils::IntegratorConstructor
{
public:
    MOCK_CONST_METHOD0(get_class_name, const char *(void));
    MOCK_CONST_METHOD0(get_transition_table_size, unsigned int(void));
    MOCK_CONST_METHOD0(create_copy, er7_utils::IntegratorConstructor *(void));
    MOCK_CONST_METHOD0(create_integration_controls, er7_utils::IntegrationControls *(void));
};

#endif /* JEOD_MODELS_UTILS_INTEGRATION_VERIF_MOCK_INTEGRATOR_CONSTRUCTOR_MOCK_HH_ */
