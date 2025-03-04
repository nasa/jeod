/*
 * spice_ephem_point_ut.cc
 */

#include "environment/spice/include/spice_ephem_point.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

class SpiceEphemPointTest : public SpiceEphemPoint
{
public:
    void set_status_test(Status testStatus)
    {
        status = testStatus;
    }

    Status get_status_test()
    {
        return status;
    }

    void set_spice_id_test(int testVal)
    {
        spice_id = testVal;
    }

    int get_spice_id_test()
    {
        return spice_id;
    }

    void set_parent_id_test(int testVal)
    {
        parent_id = testVal;
    }

    int get_parent_id_test()
    {
        return parent_id;
    }
};

TEST(SpiceEphemPoint, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemPoint staticInst;
    SpiceEphemPoint * dynInst = new SpiceEphemPoint;
    delete dynInst;
}

TEST(SpiceEphemPoint, set_status)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemPointTest staticInst;

    EXPECT_EQ(staticInst.get_status_test(), SpiceEphemPoint::Status::Active);
    staticInst.set_status(SpiceEphemPoint::Status::IsRoot);
    EXPECT_EQ(staticInst.get_status_test(), SpiceEphemPoint::Status::IsRoot);
}

TEST(SpiceEphemPoint, get_status)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemPointTest staticInst;

    EXPECT_EQ(staticInst.get_status(), SpiceEphemPoint::Status::Active);
    staticInst.set_status_test(SpiceEphemPoint::Status::IsRoot);
    EXPECT_EQ(staticInst.get_status(), SpiceEphemPoint::Status::IsRoot);
}

TEST(SpiceEphemPoint, set_spice_id)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemPointTest staticInst;

    EXPECT_EQ(staticInst.get_spice_id_test(), 32767);
    staticInst.set_spice_id(123);
    EXPECT_EQ(staticInst.get_spice_id_test(), 123);
}

TEST(SpiceEphemPoint, get_spice_id)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemPointTest staticInst;

    EXPECT_EQ(staticInst.get_spice_id(), 32767);
    staticInst.set_spice_id_test(123);
    EXPECT_EQ(staticInst.get_spice_id(), 123);
}

TEST(SpiceEphemPoint, set_parent_id)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemPointTest staticInst;

    EXPECT_EQ(staticInst.get_parent_id_test(), 32767);
    staticInst.set_parent_id(123);
    EXPECT_EQ(staticInst.get_parent_id_test(), 123);
}

TEST(SpiceEphemPoint, get_parent_id)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemPointTest staticInst;

    EXPECT_EQ(staticInst.get_parent_id(), 32767);
    staticInst.set_parent_id_test(123);
    EXPECT_EQ(staticInst.get_parent_id(), 123);
}
