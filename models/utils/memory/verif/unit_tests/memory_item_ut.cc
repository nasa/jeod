/*
 * memory_item_ut.cc
 */

#include "utils/memory/include/memory_item.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(JeodMemoryItem, create)
{
    JeodMemoryItem staticInst;
    JeodMemoryItem * dynInst = new JeodMemoryItem;
    delete dynInst;
}

TEST(JeodMemoryItem, construct_flags) {}

TEST(JeodMemoryItem, set_unique_id) {}

TEST(JeodMemoryItem, set_is_registered) {}
