/*
 * body_ref_frame_mock.hh
 *
 *  Created on: Nov 8, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_DYNAMICS_DYN_BODY_VERIF_MOCK_BODY_REF_FRAME_MOCK_HH_
#define JEOD_MODELS_DYNAMICS_DYN_BODY_VERIF_MOCK_BODY_REF_FRAME_MOCK_HH_

#include "gmock/gmock.h"

#include "dynamics/dyn_body/include/body_ref_frame.hh"

class MockBodyRefFrame : public jeod::BodyRefFrame
{
public:
    MOCK_CONST_METHOD2(compute_relative_state, void(const jeod::RefFrame & wrt_frame, jeod::RefFrameState & rel_state));
};

#endif /* JEOD_MODELS_DYNAMICS_DYN_BODY_VERIF_MOCK_BODY_REF_FRAME_MOCK_HH_ */
