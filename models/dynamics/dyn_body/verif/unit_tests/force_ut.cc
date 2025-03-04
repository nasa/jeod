/*
 * force_ut.cc
 */

#include "dynamics/dyn_body/include/force.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(CollectForce, create)
{
    CollectForce staticInst;
    CollectForce * dynInst = new CollectForce;
    delete dynInst;
}

TEST(CInterfaceForce, create)
{
    CInterfaceForce staticInst;
    CInterfaceForce * dynInst = new CInterfaceForce;
    delete dynInst;
}
