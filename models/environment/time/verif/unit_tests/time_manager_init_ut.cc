/*
 * time_manager_init_ut.cc
 */

#include "environment/time/include/time_manager_init.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TimeManagerInit, create)
{
    TimeManagerInit staticInst;
    TimeManagerInit * dynInst = new TimeManagerInit;
    delete dynInst;
}

TEST(TimeManagerInit, initialize_manager) {}

TEST(TimeManagerInit, initialize) {}

TEST(TimeManagerInit, verify_times_setup) {}

TEST(TimeManagerInit, populate_converter_registry) {}

TEST(TimeManagerInit, verify_converter_setup) {}

TEST(TimeManagerInit, create_init_tree) {}

TEST(TimeManagerInit, initialize_time_types) {}

TEST(TimeManagerInit, create_update_tree) {}

TEST(TimeManagerInit, organize_update_list) {}

TEST(TimeManagerInit, get_conv_ptr_index) {}

TEST(TimeManagerInit, get_conv_dir_init) {}

TEST(TimeManagerInit, get_conv_dir_upd) {}

TEST(TimeManagerInit, get_status) {}

TEST(TimeManagerInit, set_status) {}

TEST(TimeManagerInit, increment_status) {}
