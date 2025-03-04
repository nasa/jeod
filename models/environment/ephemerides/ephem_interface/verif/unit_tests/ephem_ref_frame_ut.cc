/*
 * ephem_ref_frame_ut.cc
 */

#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(EphemerisRefFrame, create)
{
    EphemerisRefFrame staticInst;
    EphemerisRefFrame * dynInst = new EphemerisRefFrame;
    delete dynInst;
}

TEST(EphemerisRefFrame, set_ephem_manager) {}

TEST(EphemerisRefFrame, set_active_status) {}
