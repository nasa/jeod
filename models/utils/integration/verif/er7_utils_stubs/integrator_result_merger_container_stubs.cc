/*
 * integrator_result_merger_container_stubs.cc
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */
#include "er7_utils/integration/core/include/integrator_result_merger_container.hh"
#include "utils/sim_interface/include/config.hh"

namespace er7_utils
{

IntegratorResultMergerContainer::IntegratorResultMergerContainer()
    : integ_merger(nullptr)
{
}

IntegratorResultMergerContainer::~IntegratorResultMergerContainer() {}

void IntegratorResultMergerContainer::configure(const er7_utils::IntegratorConstructor & integ_cotr JEOD_UNUSED) {}

} // namespace er7_utils
