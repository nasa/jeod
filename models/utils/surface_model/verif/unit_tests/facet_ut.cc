/*
 * facet_ut.cc
 */

#include "utils/surface_model/include/facet.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(Facet, create)
{
    Facet staticInst;
    Facet * dynInst = new Facet;
    delete dynInst;
}

TEST(Facet, initialize_mass_connection) {}

TEST(Facet, update_articulation) {}

TEST(Facet, get_mass_body_ptr) {}

TEST(Facet, update_articulation_internal) {}
