/*
 * simulation_interface_mock.hh
 *
 *  Created on: Nov 7, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_SIM_INTERFACE_VERIF_MOCK_SIMULATION_INTERFACE_MOCK_HH_
#define JEOD_MODELS_UTILS_SIM_INTERFACE_VERIF_MOCK_SIMULATION_INTERFACE_MOCK_HH_

#include "gmock/gmock.h"
using testing::AnyNumber;
using testing::ReturnRef;

#include "utils/sim_interface/include/simulation_interface.hh"

class MockJeodSimulationInterface : public jeod::JeodSimulationInterface
{
public:
    MockJeodSimulationInterface(jeod::JeodMemoryInterface & memInterfaceIn)
        : jeod::JeodSimulationInterface(),
          memInterface(memInterfaceIn)
    {
        ON_CALL(*this, get_memory_interface_internal()).WillByDefault(ReturnRef(memInterface));
        EXPECT_CALL(*this, get_memory_interface_internal()).Times(AnyNumber());
    }

    MOCK_METHOD0(create_integrator_internal, jeod::JeodIntegratorInterface *());
    MOCK_METHOD0(get_job_cycle_internal, double());
    MOCK_METHOD0(get_memory_interface_internal, jeod::JeodMemoryInterface &());
    MOCK_METHOD1(get_checkpoint_reader_internal, jeod::SectionedInputStream(const std::string & section_id));
    MOCK_METHOD1(get_checkpoint_writer_internal, jeod::SectionedOutputStream(const std::string & section_id));

protected:
    jeod::JeodMemoryInterface & memInterface;
};

#endif /* JEOD_MODELS_UTILS_SIM_INTERFACE_VERIF_MOCK_SIMULATION_INTERFACE_MOCK_HH_ */
