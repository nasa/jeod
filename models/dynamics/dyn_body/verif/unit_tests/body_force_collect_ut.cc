/*
 * body_force_collect_ut.cc
 *
 *  Created on: Nov 30, 2023
 *      Author: tbrain
 */

#include "dyn_manager_mock.hh"
#include "dynamics/dyn_body/include/body_force_collect.hh"
#include "memory_interface_mock.hh"
#include "message_handler_mock.hh"
#include "simulation_interface_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::Mock;
using testing::Return;
using testing::ReturnRef;
// using testing::Throw;

using namespace jeod;

template<typename T> class TestVector : public JeodPointerVector<T>::type
{
    template<typename CollectType, typename value_type>
    friend void jeod::collect_insert(CollectType & collect_in, value_type & elem);

    template<typename CollectType, typename value_type>
    friend void jeod::collect_push_back(CollectType & collect_in, value_type & elem);

public:
    void collect_push_back(T *& elem)
    {
        jeod::collect_push_back(*this, elem);
    }

    void collect_insert(T *& elem)
    {
        jeod::collect_insert(*this, elem);
    }

    void release_vector()
    {
        jeod::release_vector(*this);
    }
};

TEST(BodyForceCollect, create)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;

    BodyForceCollect staticInst;
    BodyForceCollect * dynInst;
    dynInst = new BodyForceCollect();

    for(int ii = 0; ii < 3; ++ii)
    {
        EXPECT_EQ(0.0, dynInst->effector_forc[ii]);
        EXPECT_EQ(0.0, dynInst->environ_forc[ii]);
        EXPECT_EQ(0.0, dynInst->no_xmit_forc[ii]);
        EXPECT_EQ(0.0, dynInst->extern_forc_struct[ii]);
        EXPECT_EQ(0.0, dynInst->extern_forc_inrtl[ii]);
        EXPECT_EQ(0.0, dynInst->effector_torq[ii]);
        EXPECT_EQ(0.0, dynInst->environ_torq[ii]);
        EXPECT_EQ(0.0, dynInst->no_xmit_torq[ii]);
        EXPECT_EQ(0.0, dynInst->inertial_torq[ii]);
        EXPECT_EQ(0.0, dynInst->extern_torq_struct[ii]);
        EXPECT_EQ(0.0, dynInst->extern_torq_body[ii]);
    }

    delete dynInst;
    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(jeod, release_vector)
{
    MockMessageHandler mockMessageHandler;
    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Test no items
        TestVector<int> testContainer;
        testContainer.release_vector();
    }
    {
        // Test some items but not jeod-allocated
        TestVector<int> testContainer;
        int var[3];
        testContainer.push_back(&var[0]);
        testContainer.push_back(&var[1]);
        testContainer.push_back(&var[2]);
        testContainer.release_vector();
    }

    {
        ON_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).WillByDefault(Return(true));
        EXPECT_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).Times(2);

        // Test some items and jeod-allocated
        TestVector<int> testContainer;
        testContainer.push_back(JEOD_ALLOC_PRIM_OBJECT(int, 0));
        testContainer.push_back(nullptr);
        testContainer.push_back(JEOD_ALLOC_PRIM_OBJECT(int, 2));
        EXPECT_EQ(3, testContainer.size());
        Mock::VerifyAndClear(&mockMemoryInterface);

        EXPECT_CALL(mockMemoryInterface, deregister_allocation(_, _, _, _, _)).Times(2);
        testContainer.release_vector();
        EXPECT_EQ(0, testContainer.size());
        Mock::VerifyAndClear(&mockMemoryInterface);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(jeod, collect_insert)
{
    MockMessageHandler mockMessageHandler;
    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Test no existing items, item added
        TestVector<int> testContainer;
        int var[3];
        int * varPtr = &var[0];
        testContainer.collect_insert(varPtr);
        EXPECT_EQ(1, testContainer.size());
        EXPECT_EQ(&var[0], testContainer[0]);
    }

    {
        // Test existing items, non-duplicate item added
        TestVector<int> testContainer;
        int var[3];
        int * varPtr = &var[0];
        testContainer.collect_insert(varPtr);
        varPtr = &var[1];
        testContainer.collect_insert(varPtr);
        EXPECT_EQ(2, testContainer.size());
        EXPECT_EQ(&var[0], testContainer[0]);
        EXPECT_EQ(&var[1], testContainer[1]);
    }

    {
        // Test existing items, duplicate jeod-allocated item added
        ON_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).WillByDefault(Return(true));
        EXPECT_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).Times(1);

        TestVector<int> testContainer;
        int * varPtr = JEOD_ALLOC_PRIM_OBJECT(int, 0);
        testContainer.collect_insert(varPtr);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Notice, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockMemoryInterface, deregister_allocation(_, _, _, _, _)).Times(1);
        testContainer.collect_insert(varPtr);
        EXPECT_EQ(1, testContainer.size());
        EXPECT_EQ(varPtr, testContainer[0]);
        Mock::VerifyAndClear(&mockMemoryInterface);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test existing items, duplicate non-jeod-allocated item added
        TestVector<int> testContainer;
        int var[3];
        int * varPtr = &var[0];
        testContainer.collect_insert(varPtr);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Notice, _, _, _, _, _, _)).Times(1);
        testContainer.collect_insert(varPtr);
        EXPECT_EQ(1, testContainer.size());
        EXPECT_EQ(varPtr, testContainer[0]);
        Mock::VerifyAndClear(&mockMemoryInterface);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(jeod, collect_push_back)
{
    MockMessageHandler mockMessageHandler;
    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        // Test no existing items, item added
        TestVector<int> testContainer;
        int var[3];
        int * varPtr = &var[0];
        testContainer.collect_push_back(varPtr);
        EXPECT_EQ(1, testContainer.size());
        EXPECT_EQ(&var[0], testContainer[0]);
    }

    {
        // Test existing items, non-duplicate item added
        TestVector<int> testContainer;
        int var[3];
        int * varPtr = &var[0];
        testContainer.collect_push_back(varPtr);
        varPtr = &var[1];
        testContainer.collect_push_back(varPtr);
        EXPECT_EQ(2, testContainer.size());
        EXPECT_EQ(&var[0], testContainer[0]);
        EXPECT_EQ(&var[1], testContainer[1]);
    }

    {
        // Test existing items, duplicate jeod-allocated item added
        ON_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).WillByDefault(Return(true));
        EXPECT_CALL(mockMemoryInterface, register_allocation(_, _, _, _, _)).Times(1);

        TestVector<int> testContainer;
        int * varPtr = JEOD_ALLOC_PRIM_OBJECT(int, 0);
        testContainer.collect_push_back(varPtr);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Notice, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockMemoryInterface, deregister_allocation(_, _, _, _, _)).Times(1);
        testContainer.collect_push_back(varPtr);
        EXPECT_EQ(1, testContainer.size());
        EXPECT_EQ(varPtr, testContainer[0]);
        Mock::VerifyAndClear(&mockMemoryInterface);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test existing items, duplicate non-jeod-allocated item added
        TestVector<int> testContainer;
        int var[3];
        int * varPtr = &var[0];
        testContainer.collect_push_back(varPtr);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Notice, _, _, _, _, _, _)).Times(1);
        testContainer.collect_push_back(varPtr);
        EXPECT_EQ(1, testContainer.size());
        EXPECT_EQ(varPtr, testContainer[0]);
        Mock::VerifyAndClear(&mockMemoryInterface);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(JPVCollectForce, perform_insert_action)
{
    MockMessageHandler mockMessageHandler;
    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        CollectForce forceInst;
        EXPECT_CALL(mockMemoryInterface, get_address_at_name(_))
            .Times(2)
            .WillOnce(Return(&forceInst))
            .WillOnce(Return(nullptr));

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(1);
        JPVCollectForce staticInst;
        staticInst.perform_insert_action("test");
        EXPECT_EQ(1, staticInst.size());
        EXPECT_EQ(&forceInst, staticInst[0]);
        staticInst.perform_insert_action("null");
        Mock::VerifyAndClear(&mockMemoryInterface);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(JPVCollectTorque, perform_insert_action)
{
    MockMessageHandler mockMessageHandler;
    MockJeodMemoryInterface mockMemoryInterface;
    MockJeodSimulationInterface mockSimInterface(mockMemoryInterface);
    JeodMemoryManager memoryManager(mockMemoryInterface);

    {
        CollectTorque trqInst;
        EXPECT_CALL(mockMemoryInterface, get_address_at_name(_))
            .Times(2)
            .WillOnce(Return(&trqInst))
            .WillOnce(Return(nullptr));

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Warning, _, _, _, _, _, _)).Times(1);
        JPVCollectTorque staticInst;
        staticInst.perform_insert_action("test");
        EXPECT_EQ(1, staticInst.size());
        EXPECT_EQ(&trqInst, staticInst[0]);
        staticInst.perform_insert_action("null");
        Mock::VerifyAndClear(&mockMemoryInterface);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
