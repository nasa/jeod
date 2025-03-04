/*
 * orbital_elements_ut.cc
 */

#include "utils/orbital_elements/include/orbital_elements.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

class OrbitalElementsTest : public OrbitalElements
{
public:
    void set_obj_name(const std::string & new_name)
    {
        object_name = new_name;
    }

    void set_plan_name(const std::string & new_name)
    {
        planet_name = new_name;
    }
};

TEST(OrbitalElements, create)
{
    OrbitalElements staticInst;
    OrbitalElements * dynInst = new OrbitalElements;
    delete dynInst;
}

TEST(OrbitalElements, set_object_name)
{
    OrbitalElementsTest staticInst;
    staticInst.set_object_name("Test3");
    EXPECT_EQ(staticInst.get_object_name(), "Test3");
    staticInst.set_object_name("");
    EXPECT_EQ(staticInst.get_object_name(), "Test3");
}

TEST(OrbitalElements, set_planet_name)
{
    OrbitalElementsTest staticInst;
    staticInst.set_planet_name("Test4");
    EXPECT_EQ(staticInst.get_planet_name(), "Test4");
    staticInst.set_planet_name("");
    EXPECT_EQ(staticInst.get_planet_name(), "Test4");
}

TEST(OrbitalElements, get_object_name)
{
    OrbitalElementsTest staticInst;
    staticInst.set_obj_name("Test1");
    EXPECT_EQ(staticInst.get_object_name(), "Test1");
}

TEST(OrbitalElements, get_planet_name)
{
    OrbitalElementsTest staticInst;
    staticInst.set_plan_name("Test2");
    EXPECT_EQ(staticInst.get_planet_name(), "Test2");
}

TEST(OrbitalElements, from_cartesian) {}

TEST(OrbitalElements, to_cartesian) {}

TEST(OrbitalElements, nu_to_anomalies) {}

TEST(OrbitalElements, mean_anom_to_nu) {}

TEST(OrbitalElements, KepEqtnE) {}

TEST(OrbitalElements, KepEqtnH) {}

TEST(OrbitalElements, KepEqtnB) {}
