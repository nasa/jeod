/*
 * mass_body_mock.hh
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_DYNAMICS_MASS_VERIF_MOCK_MASS_MOCK_HH_
#define JEOD_MODELS_DYNAMICS_MASS_VERIF_MOCK_MASS_MOCK_HH_

#include "gmock/gmock.h"

#include "dynamics/mass/include/mass.hh"
#include "dynamics/mass/include/mass_point_init.hh"
#include "dynamics/mass/include/mass_properties_init.hh"

class MockMassBody : public jeod::MassBody
{
public:
    MOCK_METHOD3(attach_to,
                 bool(const std::string & this_point_name, const std::string & parent_point_name, MassBody & parent));
    MOCK_METHOD3(attach_to, bool(double offset_pstr_cstr_pstr[3], double T_pstr_cstr[3][3], MassBody & parent));
    MOCK_METHOD0(detach, bool());
    MOCK_METHOD1(detach, bool(MassBody & parent));
    MOCK_METHOD2(reattach, bool(double offset_pstr_cstr_pstr[3], double T_pstr_cstr[3][3]));
    MOCK_METHOD3(initialize_mass,
                 void(const jeod::MassPropertiesInit properties,
                      const jeod::MassPointInit points,
                      unsigned int num_points));
};

#endif /* JEOD_MODELS_DYNAMICS_MASS_VERIF_MOCK_MASS_MOCK_HH_ */
