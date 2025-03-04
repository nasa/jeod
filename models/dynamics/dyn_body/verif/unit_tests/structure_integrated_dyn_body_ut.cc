/*
 * structure_integrated_dyn_body_ut.cc
 */

#include "dynamics/dyn_body/include/structure_integrated_dyn_body.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(StructureIntegratedDynBody, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    StructureIntegratedDynBody staticInst;
    StructureIntegratedDynBody * dynInst = new StructureIntegratedDynBody;
    delete dynInst;
}
