/*
 * checkpoint_output_manager_ut.cc
 */

#include "utils/sim_interface/include/checkpoint_output_manager.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(SectionedOutputBuffer, activate) {}

TEST(SectionedOutputBuffer, overflow) {}

TEST(SectionedOutputStream, create)
{
    SectionedOutputStream staticInst;
    SectionedOutputStream * dynInst = new SectionedOutputStream;
    delete dynInst;
}

TEST(SectionedOutputStream, is_activatable) {}

TEST(SectionedOutputStream, activate) {}

TEST(SectionedOutputStream, deactivate) {}

TEST(CheckPointOutputManager, create) {}

TEST(CheckPointInputManager, create_section_writer) {}

TEST(CheckPointInputManager, create_trick_section_writer) {}

TEST(CheckPointInputManager, register_writer) {}

TEST(CheckPointInputManager, deregister_writer) {}
