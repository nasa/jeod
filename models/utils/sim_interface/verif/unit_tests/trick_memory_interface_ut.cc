/*
 * trick_memory_interface_ut.cc
 */

#ifdef TRICK_VER

#include "utils/sim_interface/include/trick_memory_interface.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(JeodTrickMemoryInterface, create)
{
    JeodTrickMemoryInterface staticInst;
    JeodTrickMemoryInterface * dynInst = new JeodTrickMemoryInterface;
    delete dynInst;
}

TEST(JeodTrickMemoryInterface, set_mode) {}

TEST(JeodTrickMemoryInterface, construct_identifier) {}

TEST(JeodTrickMemoryInterface, register_container) {}

TEST(JeodTrickMemoryInterface, deregister_container) {}

TEST(JeodTrickMemoryInterface, get_name_at_address) {}

TEST(JeodTrickMemoryInterface, get_address_at_name) {}

#endif
