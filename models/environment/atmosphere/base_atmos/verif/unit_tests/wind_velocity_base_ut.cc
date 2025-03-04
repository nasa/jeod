/*
 * wind_velocity_base_ut.cc
 */

#include "environment/atmosphere/base_atmos/include/wind_velocity_base.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(WindVelocityBase, create)
{
    WindVelocityBase staticInst;
    WindVelocityBase * dynInst = new WindVelocityBase;
    delete dynInst;
}

TEST(WindVelocityBase, update_wind) {}
