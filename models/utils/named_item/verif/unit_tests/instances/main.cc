/*
Purpose:
  (Test the non-static interfaces to the Orientation class.
   The test driver is admittedly bizarre and twisted.
   The intent is to force coverage of all orientation.cc code except for
   the error handling.)
*/

#include "utils/named_item/include/named_item.hh"

#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/color_text.hh"
#include "test_harness/include/test_sim_interface.hh"

#include "trick_utils/math/include/trick_math.h"

#include <cstdio>
#include <string>

using namespace jeod;

bool verbose;
ColorText color;
unsigned int tests_made = 0;
unsigned int tests_passed = 0;

void test_name(const NamedItem &, const std::string &);
void update_results(bool);

void die(const char * msg)
{
    printf("%s\n", msg);
    exit(1);
}

int main(int argc, char * argv[])
{
    CmdlineParser cmdline_parser;

    TestSimInterface sim_interface;

    cmdline_parser.add_switch("verbose", &verbose);
    cmdline_parser.parse(argc, argv);

    if(verbose)
    {
        std::printf("Testing NamedItem\n");
    }
    NamedItem named_item;
    test_name(named_item, "");

    named_item.set_name("foo");
    test_name(named_item, "foo");

    named_item.set_name("foo", "bar");
    test_name(named_item, "foo.bar");

    std::printf("Ran %u tests; number failures = %u\n", tests_made, tests_made - tests_passed);

    if(tests_passed == tests_made)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void test_name(const NamedItem & named_item, const std::string & expected)
{
    update_results(named_item.get_name() == expected);
}

void update_results(bool passed)
{
    tests_made++;
    if(passed)
    {
        tests_passed++;
    }
}
