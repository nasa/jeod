/*
 * ephem_ref_frame_mock.hh
 *
 *  Created on: Aug 5, 2024
 *      Author: lmccartney
 */

#ifndef JEOD_MODELS_ENVIRONMENT_EPHEMERIDES_VERIF_MOCK_EPHEM_REF_FRAME_HH_
#define JEOD_MODELS_ENVIRONMENT_EPHEMERIDES_VERIF_MOCK_EPHEM_REF_FRAME_HH_

#include "gmock/gmock.h"

#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"

class MockEphemerisRefFrame : public jeod::EphemerisRefFrame
{
public:
    MOCK_METHOD0(subscribe, void());
    MOCK_METHOD0(unsubscribe, void());
};
#endif
