/*
 * memory_manager_protected_ut.cc
 */

#include "utils/memory/include/memory_manager.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(JeodMemoryManager, begin_atomic_block) {}

TEST(JeodMemoryManager, end_atomic_block) {}

TEST(JeodMemoryManager, get_string_atomic) {}

TEST(JeodMemoryManager, add_string_atomic) {}

TEST(JeodMemoryManager, get_type_index_nolock) {}

TEST(JeodMemoryManager, get_type_entry_atomic) {}

TEST(JeodMemoryManager, get_type_descriptor_atomic) {}

TEST(JeodMemoryManager, get_alloc_id_atomic) {}

TEST(JeodMemoryManager, reset_alloc_id_atomic) {}

TEST(JeodMemoryManager, find_alloc_entry_atomic) {}

TEST(JeodMemoryManager, add_allocation_atomic) {}

TEST(JeodMemoryManager, delete_oldest_alloc_entry_atomic) {}
