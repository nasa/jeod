/*
 * foo_ut.cc
 */

#include "utils/model_template/include/foo.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(Foo, create)
{
    Foo staticInst;
    Foo * dynInst = new Foo;
    delete dynInst;
}

TEST(Foo, set_num) {}

TEST(Foo, get_num) {}
