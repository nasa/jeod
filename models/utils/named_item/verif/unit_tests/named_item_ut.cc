/*
 * named_item_ut.cc
 */

#include "utils/named_item/include/named_item.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(NamedItem, create)
{
    NamedItem staticInst;
    NamedItem * dynInst = new NamedItem;
    delete dynInst;
}

TEST(NamedItem, suffix) {}

TEST(NamedItem, validate_name) {}

TEST(NamedItem, verify_unfrozen_name) {}
