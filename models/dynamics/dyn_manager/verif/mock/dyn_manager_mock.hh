/*
 * dyn_manager_mock.hh
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_DYNAMICS_DYN_MANAGER_VERIF_MOCK_DYN_MANAGER_MOCK_HH_
#define JEOD_MODELS_DYNAMICS_DYN_MANAGER_VERIF_MOCK_DYN_MANAGER_MOCK_HH_

#include "gmock/gmock.h"

#include "dynamics/dyn_manager/include/dyn_manager.hh"

class MockDynManager : public jeod::DynManager
{
public:
    MOCK_CONST_METHOD1(find_planet, jeod::Planet *(const std::string & name));
    MOCK_CONST_METHOD1(find_ref_frame, jeod::RefFrame *(const std::string & name));
    MOCK_CONST_METHOD1(find_integ_frame, jeod::EphemerisRefFrame *(const std::string & name));
    MOCK_CONST_METHOD1(find_dyn_body, jeod::DynBody *(const std::string & name));
    MOCK_METHOD1(remove_ref_frame, void(jeod::RefFrame & ref_frame));
    MOCK_METHOD1(add_ref_frame, void(jeod::RefFrame & ref_frame));
    MOCK_CONST_METHOD1(find_base_planet, jeod::BasePlanet *(const std::string & name));
    MOCK_METHOD1(subscribe_to_frame, void(jeod::RefFrame & ref_frame));
    MOCK_METHOD1(unsubscribe_to_frame, void(jeod::RefFrame & ref_frame));
    MOCK_CONST_METHOD1(is_dyn_body_registered, bool(const jeod::DynBody * dyn_body));
    MOCK_METHOD0(ephem_note_tree_status_change, void());
};

#endif /* JEOD_MODELS_DYNAMICS_DYN_MANAGER_VERIF_MOCK_DYN_MANAGER_MOCK_HH_ */
