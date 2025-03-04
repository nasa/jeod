/*
 * trick_message_handler_ut.cc
 */

#ifdef TRICK_VER

#include "utils/sim_interface/include/trick_message_handler.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

TEST(TrickMessageHandler, create)
{
    TrickMessageHandler staticInst;
    TrickMessageHandler * dynInst = new TrickMessageHandler;
    delete dynInst;
}

TEST(TrickMessageHandler, register_contents) {}

TEST(TrickMessageHandler, process_message) {}

#endif
