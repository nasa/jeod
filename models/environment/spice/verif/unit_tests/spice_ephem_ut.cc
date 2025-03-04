/*
 * spice_ephem_ut.cc
 */

#include "environment/spice/include/spice_ephem.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(SpiceEphemeris, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemeris staticInst;
    SpiceEphemeris * dynInst = new SpiceEphemeris;
    delete dynInst;
}

TEST(SpiceEphemeris, activate) {}

TEST(SpiceEphemeris, deactivate) {}

TEST(SpiceEphemeris, timestamp) {}

TEST(SpiceEphemeris, get_name) {}

TEST(SpiceEphemeris, initialize_model) {}

TEST(SpiceEphemeris, simple_restore) {}

TEST(SpiceEphemeris, initialize_time) {}

TEST(SpiceEphemeris, load_spice_files) {}

TEST(SpiceEphemeris, process_spk) {}

TEST(SpiceEphemeris, process_orientations) {}

TEST(SpiceEphemeris, populate_item) {}

TEST(SpiceEphemeris, create_new_ephem_point) {}

TEST(SpiceEphemeris, create_new_ephem_orientation) {}

TEST(SpiceEphemeris, initialize_items) {}

TEST(SpiceEphemeris, introduce_item) {}

TEST(SpiceEphemeris, spice_2_jeod) {}

TEST(SpiceEphemeris, jeod_2_spice_pfix) {}

TEST(SpiceEphemeris, name_barycenter_frames) {}

TEST(SpiceEphemeris, add_barycenter) {}

TEST(SpiceEphemeris, create_barycenters) {}

TEST(SpiceEphemeris, determine_root_node) {}

TEST(SpiceEphemeris, ephem_initialize) {}

TEST(SpiceEphemeris, ephem_activate) {}

TEST(SpiceEphemeris, add_descendants_r) {}

TEST(SpiceEphemeris, ephem_build_tree) {}

TEST(SpiceEphemeris, ephem_update) {}

TEST(SpiceEphemeris, find_spice_id) {}

TEST(SpiceEphemeris, find_parent_id) {}

TEST(SpiceEphemeris, update_trans) {}

TEST(SpiceEphemeris, update_rot) {}

TEST(SpiceEphemeris, mute_spice_errors) {}
