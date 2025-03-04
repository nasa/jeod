/*
 * radiation_source_ut.cc
 */

#include "interactions/radiation_pressure/include/radiation_source.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(RadiationSource, create)
{
    RadiationSource staticInst;
    RadiationSource * dynInst = new RadiationSource;
    delete dynInst;
}

TEST(RadiationSource, calculate_flux) {}

TEST(RadiationSource, initialize) {}
