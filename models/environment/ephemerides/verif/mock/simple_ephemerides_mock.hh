/*
 * simple_ephemerides_mock.hh
 *
 *  Created on: Dec 13, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_ENVIRONMENT_EPHEMERIDES_VERIF_MOCK_SIMPLE_EPHEMERIDES_MOCK_HH_
#define JEOD_MODELS_ENVIRONMENT_EPHEMERIDES_VERIF_MOCK_SIMPLE_EPHEMERIDES_MOCK_HH_

#include "gmock/gmock.h"

#include "environment/ephemerides/ephem_interface/include/simple_ephemerides.hh"

class MockSinglePointEphemeris : public jeod::SinglePointEphemeris
{
public:
    MOCK_METHOD1(initialize_model, void(jeod::EphemeridesManager & manager));
    MOCK_METHOD1(ephem_initialize, void(jeod::EphemeridesManager & manager));
    MOCK_METHOD1(ephem_activate, void(jeod::EphemeridesManager & manager));
    MOCK_METHOD1(ephem_build_tree, void(jeod::EphemeridesManager & manager));
};

#endif /* JEOD_MODELS_ENVIRONMENT_EPHEMERIDES_VERIF_MOCK_SIMPLE_EPHEMERIDES_MOCK_HH_ */
