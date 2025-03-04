/*
 * checkpoint_input_manager_ut.cc
 */

#include "utils/sim_interface/include/checkpoint_input_manager.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(SectionedInputBuffer, create) {}

TEST(SectionedInputBuffer, activate) {}

TEST(SectionedInputBuffer, underflow) {}

TEST(SectionedInputStream, create)
{
    SectionedInputStream staticInst;
    SectionedInputStream * dynInst = new SectionedInputStream;
    delete dynInst;
}

TEST(SectionedInputStream, is_activatable) {}

TEST(SectionedInputStream, activate) {}

TEST(SectionedInputStream, deactivate) {}

TEST(CheckPointInputManager, create) {}

TEST(CheckPointInputManager, initialize) {}

TEST(CheckPointInputManager, create_section_reader) {}

TEST(CheckPointInputManager, create_trick_section_reader) {}

TEST(CheckPointInputManager, register_reader) {}

TEST(CheckPointInputManager, deregister_reader) {}
