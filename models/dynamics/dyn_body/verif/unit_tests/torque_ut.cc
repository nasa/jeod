/*
 * torque_ut.cc
 */

#include "dynamics/dyn_body/include/torque.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(CollectTorque, create)
{
    CollectTorque staticInst;
    CollectTorque * dynInst = new CollectTorque;
    delete dynInst;
}

TEST(CInterfaceTorque, create)
{
    CInterfaceTorque staticInst;
    CInterfaceTorque * dynInst = new CInterfaceTorque;
    delete dynInst;
}
