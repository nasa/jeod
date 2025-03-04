/*
 * mass_ut.cc
 *
 *  Created on: Feb 20, 2024
 *      Author: tbrain
 */

#include "dyn_body_mock.hh"
#include "dynamics/mass/include/mass.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::HasSubstr;
using testing::Mock;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;

using namespace jeod;

class MassBodyTest : public MassBody
{
public:
    MassBodyTest()
        : MassBody()
    {
    }

    MassBodyTest(DynBody & owner)
        : MassBody(owner)
    {
    }

    BaseDynManager * get_dyn_manager()
    {
        return dyn_manager;
        ;
    }

    bool get_mass_properties_initialized()
    {
        return mass_properties_initialized;
    }

    bool get_needs_update()
    {
        return needs_update;
    }

    MassPoint & get_core_wrt_composite()
    {
        return core_wrt_composite;
    }

    MassBodyLinks & get_links()
    {
        return links;
    }

    std::list<MassPoint *> get_mass_points()
    {
        return mass_points;
    }
};

TEST(MassBody, create)
{
    MockMessageHandler mockMessageHandler;
    {
        // Default constructor test
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MassBody staticInst;
        MassBodyTest * dynInst = new MassBodyTest;
        MassBody * dynInst2 = new MassBody;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_EQ("", dynInst->name.get_name());
        EXPECT_FALSE(dynInst->compute_inverse_inertia);
        EXPECT_EQ(nullptr, dynInst->dyn_owner);
        EXPECT_EQ(nullptr, dynInst->get_dyn_manager());
        EXPECT_FALSE(dynInst->get_mass_properties_initialized());
        EXPECT_FALSE(dynInst->get_needs_update());
        for(int ii = 0; ii < 3; ++ii)
        {
            for(int jj = 0; jj < 3; ++jj)
            {
                if(ii == jj)
                {
                    EXPECT_EQ(1.0, dynInst->get_core_wrt_composite().T_parent_this[ii][jj]);
                }
                else
                {
                    EXPECT_EQ(0.0, dynInst->get_core_wrt_composite().T_parent_this[ii][jj]);
                }
            }
        }
        EXPECT_FALSE(dynInst->get_links().has_children());
        EXPECT_TRUE(dynInst->get_links().is_root());
        EXPECT_EQ(0, dynInst->get_mass_points().size());

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, HasSubstr("master memory manager"), _))
            .Times(AnyNumber());
        delete dynInst2;
        delete dynInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    {
        // DynBody owner constructor test
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        MockDynBody mockDynBody;
        MassBodyTest * dynInst = new MassBodyTest(mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_EQ("", dynInst->name.get_name());
        EXPECT_FALSE(dynInst->compute_inverse_inertia);
        EXPECT_EQ(&mockDynBody, dynInst->dyn_owner);
        EXPECT_EQ(nullptr, dynInst->get_dyn_manager());
        EXPECT_FALSE(dynInst->get_mass_properties_initialized());
        EXPECT_FALSE(dynInst->get_needs_update());
        for(int ii = 0; ii < 3; ++ii)
        {
            for(int jj = 0; jj < 3; ++jj)
            {
                if(ii == jj)
                {
                    EXPECT_EQ(1.0, dynInst->get_core_wrt_composite().T_parent_this[ii][jj]);
                }
                else
                {
                    EXPECT_EQ(0.0, dynInst->get_core_wrt_composite().T_parent_this[ii][jj]);
                }
            }
        }
        EXPECT_FALSE(dynInst->get_links().has_children());
        EXPECT_TRUE(dynInst->get_links().is_root());
        EXPECT_EQ(0, dynInst->get_mass_points().size());

        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, HasSubstr("master memory manager"), _))
            .Times(AnyNumber());
        delete dynInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(MassBody, initialize_mass) {}

TEST(MassBody, get_parent_body) {}

TEST(MassBody, get_parent_body_internal) {}

TEST(MassBody, get_root_body) {}

TEST(MassBody, get_root_body_internal) {}

TEST(MassBody, is_progeny_of) {}

TEST(MassBody, set_update_flag) {}

TEST(MassBody, mass_points_size) {}

TEST(MassBody, find_mass_point) {}

TEST(MassBody, add_mass_point) {}
