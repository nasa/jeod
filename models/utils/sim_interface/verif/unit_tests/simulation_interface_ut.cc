/*
 * simulation_interface_ut.cc
 */

#ifdef TRICK_VER

#include "utils/sim_interface/include/simulation_interface.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(JeodSimulationInterfaceInit, create)
{
    JeodSimulationInterfaceInit staticInst;
    JeodSimulationInterfaceInit * dynInst = new JeodSimulationInterfaceInit;
    delete dynInst;
}

TEST(JeodSimulationInterface, configure) {}

TEST(JeodSimulationInterface, create_integrator_interface) {}

TEST(JeodSimulationInterface, get_job_cycle) {}

TEST(JeodSimulationInterface, get_memory_interface) {}

TEST(JeodSimulationInterface, get_name_at_address) {}

TEST(JeodSimulationInterface, get_address_at_name) {}

TEST(JeodSimulationInterface, get_checkpoint_reader) {}

TEST(JeodSimulationInterface, get_checkpoint_writer) {}

TEST(JeodSimulationInterface, set_mode) {}

#endif
