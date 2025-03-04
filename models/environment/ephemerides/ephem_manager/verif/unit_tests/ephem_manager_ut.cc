/*
 * ephem_manager_ut.cc
 */

#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(EphemeridesManager, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    EphemeridesManager staticInst;
    EphemeridesManager * dynInst = new EphemeridesManager;
    delete dynInst;
}

TEST(EphemeridesManager, ephem_note_tree_status_change) {}

TEST(EphemeridesManager, find_base_planet) {}

TEST(EphemeridesManager, get_num_planets) {}

TEST(EphemeridesManager, disable_add_ephemeris) {}

TEST(EphemeridesManager, add_ephemeris) {}

TEST(EphemeridesManager, clear_added_ephemerides) {}

TEST(EphemeridesManager, add_ephem_item) {}

TEST(EphemeridesManager, find_ephem_item) {}

TEST(EphemeridesManager, find_ephem_angle) {}

TEST(EphemeridesManager, find_ephem_point) {}

TEST(EphemeridesManager, add_integ_frame) {}

TEST(EphemeridesManager, find_integ_frame) {}

TEST(EphemeridesManager, get_integ_frames) {}

TEST(EphemeridesManager, is_integ_frame) {}

TEST(EphemeridesManager, find_integ_frame_index) {}

TEST(EphemeridesManager, add_ref_frame) {}

TEST(EphemeridesManager, set_target_frame) {}

TEST(EphemeridesManager, initialize_ephemerides) {}

TEST(EphemeridesManager, update_ephemerides) {}

TEST(EphemeridesManager, activate_ephemerides) {}
