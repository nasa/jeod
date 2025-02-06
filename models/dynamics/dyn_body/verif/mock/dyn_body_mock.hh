/*
 * dyn_body_mock.hh
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_DYNAMICS_DYN_BODY_VERIF_MOCK_DYN_BODY_MOCK_HH_
#define JEOD_MODELS_DYNAMICS_DYN_BODY_VERIF_MOCK_DYN_BODY_MOCK_HH_

#include "gmock/gmock.h"

#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/gravity/include/gravity_manager.hh"

class MockDynBody : public jeod::DynBody
{
public:
    MOCK_METHOD3(attach_child,
                 bool(const std::string & this_point_name, const std::string & child_point_name, DynBody & child));
    MOCK_METHOD3(attach_child,
                 bool(const double offset_pstr_cstr_pstr[3], const double T_pstr_cstr[3][3], DynBody & child));
    MOCK_METHOD3(add_mass_body,
                 bool(const std::string & this_point_name,
                      const std::string & child_point_name,
                      jeod::MassBody & child));
    MOCK_METHOD3(add_mass_body,
                 bool(const double offset_pstr_cstr_pstr[3], const double T_pstr_cstr[3][3], jeod::MassBody & child));
    MOCK_METHOD4(attach_to_frame,
                 bool(const std::string & this_point_name,
                      const std::string & parent_ref_frame_name,
                      const double offset_pframe_cpt_pframe[3],
                      const double T_pframe_cpt[3][3]));
    MOCK_CONST_METHOD1(find_body_frame, jeod::BodyRefFrame *(const std::string & frame_id));
    MOCK_METHOD1(initialize_controls, void(jeod::GravityManager & grav_manager));
    MOCK_METHOD0(reset_controls, void());
    MOCK_METHOD1(remove_mass_body, bool(jeod::MassBody & child));
    MOCK_METHOD0(detach, bool());
    MOCK_METHOD1(detach, bool(jeod::DynBody & parent));
    MOCK_CONST_METHOD0(get_integ_frame, jeod::EphemerisRefFrame *());
    MOCK_METHOD1(switch_integration_frames, void(jeod::EphemerisRefFrame &));
};

#endif /* JEOD_MODELS_DYNAMICS_DYN_BODY_VERIF_MOCK_DYN_BODY_MOCK_HH_ */
