/*
 * ned_derived_state_ut.cc
 */

#include "dynamics/derived_state/include/ned_derived_state.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

class NedDerivedStateTest : public NedDerivedState
{
public:
    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return NedDerivedState::find_planet(dyn_manager, planet_name, variable_name);
    }

    std::string get_state_identifier()
    {
        return state_identifier;
    }

    bool get_use_alt_pfix()
    {
        return use_alt_pfix;
    }
};

TEST(NedDerivedState, create)
{
    NedDerivedState staticInst;
    NedDerivedState * dynInst = new NedDerivedState;
    delete dynInst;
}

TEST(NedDerivedState, set_use_alt_pfix)
{
    NedDerivedStateTest staticInst;

    EXPECT_FALSE(staticInst.get_use_alt_pfix());

    staticInst.set_use_alt_pfix(true);

    EXPECT_TRUE(staticInst.get_use_alt_pfix());
}

TEST(NedDerivedState, initialize) {}

TEST(NedDerivedState, update) {}

TEST(NedDerivedState, compute_ned_frame) {}
