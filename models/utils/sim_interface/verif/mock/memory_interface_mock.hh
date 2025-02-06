/*
 * memory_interface_mock.hh
 *
 *  Created on: Nov 7, 2023
 *      Author: tbrain
 */

#ifndef JEOD_MODELS_UTILS_MEMORY_VERIF_MOCK_MEMORY_INTERFACE_MOCK_HH_
#define JEOD_MODELS_UTILS_MEMORY_VERIF_MOCK_MEMORY_INTERFACE_MOCK_HH_

#include "gmock/gmock.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::Return;

#include "utils/sim_interface/include/memory_interface.hh"

class MockJeodMemoryInterface : public jeod::JeodMemoryInterface
{
public:
    MockJeodMemoryInterface()
        : jeod::JeodMemoryInterface()
    {
        ON_CALL(*this, primitive_attributes(_)).WillByDefault(Return(0));
        ON_CALL(*this, void_pointer_attributes()).WillByDefault(Return(0));
        EXPECT_CALL(*this, primitive_attributes(_)).Times(AnyNumber());
        EXPECT_CALL(*this, void_pointer_attributes()).Times(AnyNumber());
        uint64_t dummyVar;
        const void * cptr = &dummyVar;
        EXPECT_CALL(*this, find_attributes(An<const std::string &>())).Times(AnyNumber()).WillRepeatedly(Return(cptr));
        EXPECT_CALL(*this, find_attributes(An<const std::type_info &>()))
            .Times(AnyNumber())
            .WillRepeatedly(Return(cptr));
        EXPECT_CALL(*this, structure_attributes(_, _)).Times(AnyNumber()).WillRepeatedly(Return(1));
        EXPECT_CALL(*this, register_container(_, _, _, _)).Times(AnyNumber());
        EXPECT_CALL(*this, deregister_container(_, _, _, _)).Times(AnyNumber());
        EXPECT_CALL(*this, register_allocation(_, _, _, _, _)).Times(AnyNumber()).WillRepeatedly(Return(true));
        EXPECT_CALL(*this, deregister_allocation(_, _, _, _, _)).Times(AnyNumber());
    }

    MOCK_CONST_METHOD1(find_attributes, const JEOD_ATTRIBUTES_POINTER_TYPE(const std::string & type_name));
    MOCK_CONST_METHOD1(find_attributes, const JEOD_ATTRIBUTES_POINTER_TYPE(const std::type_info & data_type));
    MOCK_CONST_METHOD1(primitive_attributes, JEOD_ATTRIBUTES_TYPE(const std::type_info & data_type));
    MOCK_CONST_METHOD1(pointer_attributes, JEOD_ATTRIBUTES_TYPE(const JEOD_ATTRIBUTES_TYPE & pointed_to_attr));
    MOCK_CONST_METHOD0(void_pointer_attributes, JEOD_ATTRIBUTES_TYPE());
    MOCK_CONST_METHOD2(structure_attributes,
                       JEOD_ATTRIBUTES_TYPE(const JEOD_ATTRIBUTES_POINTER_TYPE target_attr, std::size_t target_size));
    MOCK_METHOD5(register_allocation,
                 bool(const void * addr,
                      const jeod::JeodMemoryItem & item,
                      const jeod::JeodMemoryTypeDescriptor & tdesc,
                      const char * file,
                      unsigned int line));
    MOCK_METHOD5(deregister_allocation,
                 void(const void * addr,
                      const jeod::JeodMemoryItem & item,
                      const jeod::JeodMemoryTypeDescriptor & tdesc,
                      const char * file,
                      unsigned int line));
    MOCK_METHOD4(register_container,
                 void(const void * container,
                      const jeod::JeodMemoryTypeDescriptor & container_type,
                      const std::string & elem_name,
                      jeod::JeodCheckpointable & checkpointable));
    MOCK_METHOD4(deregister_container,
                 void(const void * container,
                      const jeod::JeodMemoryTypeDescriptor & container_type,
                      const std::string & elem_name,
                      jeod::JeodCheckpointable & checkpointable));
    MOCK_CONST_METHOD0(is_checkpoint_restart_supported, bool());
    MOCK_CONST_METHOD2(get_name_at_address,
                       const std::string(const void * addr, const jeod::JeodMemoryTypeDescriptor * tdesc));
    MOCK_CONST_METHOD1(get_address_at_name, void *(const std::string & name));
};

#endif /* JEOD_MODELS_UTILS_MEMORY_VERIF_MOCK_MEMORY_INTERFACE_MOCK_HH_ */
