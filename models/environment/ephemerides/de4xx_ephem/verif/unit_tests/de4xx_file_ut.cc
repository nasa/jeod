/*
 * de4xx_file_ut.cc
 */

#include "environment/ephemerides/de4xx_ephem/include/de4xx_file.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(De4xxFileSpec, create)
{
    De4xxFileSpec staticInst;
    De4xxFileSpec * dynInst = new De4xxFileSpec;
    delete dynInst;
}

TEST(De4xxFileSpec, set_model_number) {}

TEST(De4xxFileSpec, set_model_directory) {}

TEST(De4xxFileHeader, create) {}

TEST(De4xxFileItem, create)
{
    De4xxFileItem staticInst;
    De4xxFileItem * dynInst = new De4xxFileItem;
    delete dynInst;
}

TEST(De4xxFileRestart, create) {}

TEST(De4xxFileRestart, simple_restore) {}

TEST(De4xxFile, create) {}

TEST(De4xxFile, shutdown) {}

TEST(De4xxFile, open) {}

TEST(De4xxFile, reopen) {}

TEST(De4xxFile, close) {}

TEST(De4xxFile, time_is_in_range) {}

TEST(De4xxFile, capture_mem_stats) {}
