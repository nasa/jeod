/*
 * dyn_manager_mock.hh
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_DYNAMICS_BODY_ACTION_VERIF_MOCK_BODY_ACTION_MOCK_HH_
#define JEOD_MODELS_DYNAMICS_BODY_ACTION_VERIF_MOCK_BODY_ACTION_MOCK_HH_

#include "gmock/gmock.h"

#include "dynamics/body_action/include/body_action.hh"

class MockBodyAction : public jeod::BodyAction
{
public:
    MOCK_METHOD0(shutdown, void());
};

#endif /* JEOD_MODELS_DYNAMICS_BODY_ACTION_VERIF_MOCK_BODY_ACTION_MOCK_HH_ */
