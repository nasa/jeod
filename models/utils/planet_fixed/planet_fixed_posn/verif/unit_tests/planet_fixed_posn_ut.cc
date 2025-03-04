/*
 * planet_fixed_posn_ut.cc
 */

#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(PlanetFixedPosition, create)
{
    PlanetFixedPosition staticInst;
    PlanetFixedPosition * dynInst = new PlanetFixedPosition;
    delete dynInst;
}

TEST(PlanetFixedPosition, initialize) {}

TEST(PlanetFixedPosition, update_from_cart) {}

TEST(PlanetFixedPosition, update_from_spher) {}

TEST(PlanetFixedPosition, update_from_ellip) {}

TEST(PlanetFixedPosition, cart_to_spher) {}

TEST(PlanetFixedPosition, cart_to_ellip) {}

TEST(PlanetFixedPosition, spher_to_cart) {}

TEST(PlanetFixedPosition, ellip_to_cart) {}

TEST(PlanetFixedPosition, get_elliptic_parameters) {}
