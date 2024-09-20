/*
 * dynamics_integration_group_mock.hh
 *
 *  Created on: Feb 6, 2024
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_DYNAMICS_DYN_MANAGER_VERIF_MOCK_DYNAMICS_INTEGRATION_GROUP_MOCK_HH_
#define JEOD_MODELS_DYNAMICS_DYN_MANAGER_VERIF_MOCK_DYNAMICS_INTEGRATION_GROUP_MOCK_HH_

#include "gmock/gmock.h"

#include "dynamics/dyn_manager/include/dynamics_integration_group.hh"

class MockDynamicsIntegrationGroup : public jeod::DynamicsIntegrationGroup
{
public:
    MOCK_METHOD2(gravitation, void(jeod::DynManager & dyn_manager, jeod::GravityManager & gravity_manager));
};

#endif /* JEOD_MODELS_DYNAMICS_DYN_MANAGER_VERIF_MOCK_DYNAMICS_INTEGRATION_GROUP_MOCK_HH_ */
