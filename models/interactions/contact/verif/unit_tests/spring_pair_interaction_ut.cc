/*
 * spring_pair_interaction_ut.cc
 */

#include "interactions/contact/include/spring_pair_interaction.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(SpringPairInteraction, create)
{
    SpringPairInteraction staticInst;
    SpringPairInteraction * dynInst = new SpringPairInteraction;
    delete dynInst;
}

TEST(SpringPairInteraction, calculate_forces) {}
