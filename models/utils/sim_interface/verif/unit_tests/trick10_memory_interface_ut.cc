/*
 * trick10_memory_interface_ut.cc
 */

#ifdef TRICK_VER

#include "utils/sim_interface/include/trick10_memory_interface.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(JeodTrick10MemoryInterface, create)
{
    JeodTrick10MemoryInterface staticInst;
    JeodTrick10MemoryInterface * dynInst = new JeodTrick10MemoryInterface;
    delete dynInst;
}

#endif
