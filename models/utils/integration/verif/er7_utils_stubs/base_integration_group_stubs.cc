/*
 * base_integration_group_stubs.cc
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */
#include "er7_utils/integration/core/include/base_integration_group.hh"
#include "integration_controls_mock.hh"
#include "utils/sim_interface/include/config.hh"

namespace er7_utils
{

BaseIntegrationGroup::BaseIntegrationGroup()
    : integ_constructor(nullptr),
      integ_interface(nullptr),
      time_interface(nullptr),
      integ_controls(nullptr)
{
    integ_controls = MockIntegrationControls::get_global_instance()->create_copy();
}

BaseIntegrationGroup::BaseIntegrationGroup(IntegratorConstructor & integ_cotr JEOD_UNUSED,
                                           IntegratorInterface & integ_inter JEOD_UNUSED,
                                           TimeInterface & time_if JEOD_UNUSED)
    : integ_constructor(&integ_cotr),
      integ_interface(&integ_inter),
      time_interface(&time_if),
      integ_controls(nullptr)
{
    integ_controls = MockIntegrationControls::get_global_instance()->create_copy();
}

BaseIntegrationGroup::~BaseIntegrationGroup() {}

void BaseIntegrationGroup::initialize_group() {}

void BaseIntegrationGroup::swap(BaseIntegrationGroup & other JEOD_UNUSED) {}

} // namespace er7_utils
