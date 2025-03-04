/*
 * primitive_serializer_ut.cc
 */

#include "utils/container/include/primitive_serializer.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(JeodPrimitiveSerializerBase, create)
{
    JeodPrimitiveSerializerBase staticInst;
    JeodPrimitiveSerializerBase * dynInst = new JeodPrimitiveSerializerBase;
    delete dynInst;
}

TEST(JeodPrimitiveSerializerBase, serialize_string) {}

TEST(JeodPrimitiveSerializerBase, deserialize_string) {}

TEST(JeodPrimitiveSerializerBase, serialize_float) {}

TEST(JeodPrimitiveSerializerBase, deserialize_float) {}

TEST(JeodPrimitiveSerializerBase, serialize_double) {}

TEST(JeodPrimitiveSerializerBase, deserialize_double) {}

TEST(JeodPrimitiveSerializerBase, serialize_long_double) {}

TEST(JeodPrimitiveSerializerBase, deserialize_long_double) {}
