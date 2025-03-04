/*
 * rnp_j2000_ut.cc
 */

#include "environment/RNP/RNPJ2000/include/rnp_j2000.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RNPJ2000, create)
{
    RNPJ2000 staticInst;
    RNPJ2000 * dynInst = new RNPJ2000;
    delete dynInst;
}

TEST(RNPJ2000, initialize) {}

TEST(RNPJ2000, update_rnp) {}

TEST(RNPJ2000, update_axial_rotation) {}

TEST(RNPJ2000, timestamp) {}

TEST(RNPJ2000, get_name) {}

TEST(RNPJ2000, ephem_update) {}

TEST(RNPJ2000, get_dyn_time_ptr) {}
