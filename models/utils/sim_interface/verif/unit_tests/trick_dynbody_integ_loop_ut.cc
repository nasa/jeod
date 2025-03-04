/*
 * trick_dynbody_integ_loop_ut.cc
 */

#ifdef TRICK_VER

#include "utils/sim_interface/include/trick_dynbody_integ_loop.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(JeodDynbodyIntegrationLoop, create)
{
    JeodDynbodyIntegrationLoop staticInst;
    JeodDynbodyIntegrationLoop * dynInst = new JeodDynbodyIntegrationLoop;
    delete dynInst;
}

TEST(JeodDynbodyIntegrationLoop, initialize_integ_loop) {}

TEST(JeodDynbodyIntegrationLoop, find_containing_sim_object) {}

TEST(JeodDynbodyIntegrationLoop, add_integrable_object) {}

TEST(JeodDynbodyIntegrationLoop, remove_integrable_object) {}

TEST(JeodDynbodyIntegrationLoop, set_time_to_loop_start) {}

TEST(JeodDynbodyIntegrationLoop, integrate_dt) {}

TEST(JeodDynbodyIntegrationLoop, add_sim_object) {}

TEST(JeodDynbodyIntegrationLoop, remove_sim_object) {}

TEST(JeodDynbodyIntegrationLoop, add_sim_object_bodies) {}

TEST(JeodDynbodyIntegrationLoop, remove_sim_object_bodies) {}

TEST(JeodDynbodyIntegrationLoop, update_integration_group) {}

#endif
