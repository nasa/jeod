/*
 * wind_velocity_ut.cc
 */

#include "environment/atmosphere/base_atmos/include/wind_velocity.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(WindVelocity, create)
{
    WindVelocity staticInst;
    WindVelocity * dynInst = new WindVelocity;
    delete dynInst;
}

TEST(WindVelocity, update_wind) {}

TEST(WindVelocity, get_num_layers) {}

TEST(WindVelocity, set_omega_scale_table) {}

TEST(WindVelocity, get_omega_scale_table) {}
