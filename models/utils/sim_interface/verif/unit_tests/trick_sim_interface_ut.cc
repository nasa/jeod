/*
 * trick_sim_interface_ut.cc
 */

#ifdef TRICK_VER

#include "utils/sim_interface/include/trick_sim_interface.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(BasicJeodTrickSimInterface, create)
{
    BasicJeodTrickSimInterface staticInst;
    BasicJeodTrickSimInterface * dynInst = new BasicJeodTrickSimInterface;
    delete dynInst;
}

TEST(BasicJeodTrickSimInterface, set_mode) {}

TEST(BasicJeodTrickSimInterface, create_integrator_internal) {}

TEST(BasicJeodTrickSimInterface, get_job_cycle_internal) {}

TEST(BasicJeodTrickSimInterface, get_memory_interface_internal) {}

TEST(BasicJeodTrickSimInterface, open_checkpoint_file) {}

TEST(BasicJeodTrickSimInterface, get_checkpoint_writer_internal) {}

TEST(BasicJeodTrickSimInterface, close_checkpoint_file) {}

TEST(BasicJeodTrickSimInterface, open_restart_file) {}

TEST(BasicJeodTrickSimInterface, get_checkpoint_reader_internal) {}

TEST(BasicJeodTrickSimInterface, close_restart_file) {}

TEST(BasicJeodTrickSimInterface, checkpoint_allocations) {}

TEST(BasicJeodTrickSimInterface, restore_allocations) {}

TEST(BasicJeodTrickSimInterface, checkpoint_containers) {}

TEST(BasicJeodTrickSimInterface, restore_containers) {}

#endif
