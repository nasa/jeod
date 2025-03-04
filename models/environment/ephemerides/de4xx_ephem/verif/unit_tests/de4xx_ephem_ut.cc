/*
 * de4xx_ephem_ut.cc
 */

#include "environment/ephemerides/de4xx_ephem/include/de4xx_ephem.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(De4xxEphemeris, create) {}

TEST(De4xxEphemeris, shutdown) {}

TEST(De4xxEphemeris, activate) {}

TEST(De4xxEphemeris, deactivate) {}

TEST(De4xxEphemeris, timestamp) {}

TEST(De4xxEphemeris, get_name) {}

// This is overloaded, but the other definition is in de4xx_ephem_dynmanager.cc
// TEST(De4xxEphemeris, initialize_model) {}

TEST(De4xxEphemeris, initialize_time) {}

TEST(De4xxEphemeris, initialize_file) {}

TEST(De4xxEphemeris, initialize_items) {}

TEST(De4xxEphemeris, ephem_initialize) {}

TEST(De4xxEphemeris, activate_nodes) {}

TEST(De4xxEphemeris, activate_em_nodes) {}

TEST(De4xxEphemeris, determine_root_node) {}

TEST(De4xxEphemeris, ephem_activate) {}

TEST(De4xxEphemeris, ephem_build_tree) {}

TEST(De4xxEphemeris, ephem_update) {}

TEST(De4xxEphemeris, time_is_in_range) {}

TEST(De4xxEphemeris, propagate_lunar_rnp) {}
