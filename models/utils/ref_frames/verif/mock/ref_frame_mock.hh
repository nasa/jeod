/*
 * ref_frame_mock.hh
 *
 *  Created on: Nov 8, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_REF_FRAMES_VERIF_MOCK_REF_FRAME_MOCK_HH_
#define JEOD_MODELS_UTILS_REF_FRAMES_VERIF_MOCK_REF_FRAME_MOCK_HH_

#include "gmock/gmock.h"

#include "utils/ref_frames/include/ref_frame.hh"

class MockRefFrame : public jeod::RefFrame
{
public:
    MOCK_CONST_METHOD2(compute_relative_state, void(const jeod::RefFrame & wrt_frame, jeod::RefFrameState & rel_state));
    MOCK_METHOD1(add_child, void(jeod::RefFrame & frame));

    jeod::RefFrameLinks & get_links()
    {
        return links;
    }
};

#endif /* JEOD_MODELS_UTILS_REF_FRAMES_VERIF_MOCK_REF_FRAME_MOCK_HH_ */
