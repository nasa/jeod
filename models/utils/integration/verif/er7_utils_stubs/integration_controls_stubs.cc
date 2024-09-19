/*
 * integration_controls.cc
 *
 *  Created on: Jan 15, 2024
 *      Author: tbrain
 */

#include "er7_utils/integration/core/include/integration_controls.hh"
#include "utils/sim_interface/include/config.hh"

namespace er7_utils
{
IntegrationControls::IntegrationControls() {}

IntegrationControls::~IntegrationControls() {}

void IntegrationControls::swap(IntegrationControls & other)
{
    (void)other;
}

} // namespace er7_utils
