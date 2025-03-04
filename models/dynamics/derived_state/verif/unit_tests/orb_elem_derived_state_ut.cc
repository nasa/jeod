/*
 * orb_elem_derived_state_ut.cc
 */

#include "dynamics/derived_state/include/orb_elem_derived_state.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

class OrbElemDerivedStateTest : public OrbElemDerivedState
{
public:
    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return OrbElemDerivedState::find_planet(dyn_manager, planet_name, variable_name);
    }

    std::string get_state_identifier()
    {
        return state_identifier;
    }

    bool get_use_alt_inertial()
    {
        return use_alt_inertial;
    }
};

TEST(OrbElemDerivedState, create)
{
    OrbElemDerivedState staticInst;
    OrbElemDerivedState * dynInst = new OrbElemDerivedState;
    delete dynInst;
}

TEST(OrbElemDerivedState, set_use_alt_inertial)
{
    OrbElemDerivedStateTest staticInst;

    EXPECT_FALSE(staticInst.get_use_alt_inertial());

    staticInst.set_use_alt_inertial(true);

    EXPECT_TRUE(staticInst.get_use_alt_inertial());
}

TEST(OrbElemDerivedState, initialize) {}

TEST(OrbElemDerivedState, update) {}

TEST(OrbElemDerivedState, compute_orbital_elements) {}
