/*
 * integrator_constructor_factory_stubs.cc
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#include "er7_utils/integration/core/include/integrator_constructor_factory.hh"
#include "utils/sim_interface/include/config.hh"

namespace er7_utils
{

IntegratorConstructor * IntegratorConstructorFactory::create(Integration::Technique integ_type JEOD_UNUSED)
{
    return nullptr;
}

} // namespace er7_utils
