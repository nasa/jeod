/*
 * flat_plate_aero_facet_ut.cc
 */

#include "interactions/aerodynamics/include/flat_plate_aero_facet.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(FlatPlateAeroFacet, create)
{
    FlatPlateAeroFacet staticInst;
    FlatPlateAeroFacet * dynInst = new FlatPlateAeroFacet;
    delete dynInst;
}

TEST(FlatPlateAeroFacet, aerodrag_force) {}
