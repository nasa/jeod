/*******************************************************************************

Purpose:
  (Unit test for the CheckItem class.)



*******************************************************************************/

// System includes
#include <cmath>
#include <iostream>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Test harness includes
#include "test_harness/include/check_item.hh"
#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/color_text.hh"
#include "test_harness/include/test_harness_messages.hh"
#include "test_harness/include/test_sim_interface.hh"

using namespace jeod;

int main(int argc, char ** argv)
{
    // Test harness stuff
    // Note well: The sim interface needs to be declared first.
    TestSimInterface sim_interface;
    CmdlineParser cmdline_parser;
    ColorText color;
    bool all_passed;
    bool item_passed;
    double com_vec[3];
    double exp_vec[3];
    bool out_vec[3];
    double com_mat[3][3];
    double exp_mat[3][3];
    bool out_mat[3][3];

    all_passed = true;

    // Test the scalar within_tolerance methods.
    std::cout << "Testing 9.99 versus 10"
              << "\n";

    std::cout << "To within 0.011 absolute (expect pass): ";
    item_passed = CheckItem::within_tolerance(9.99, 10, 0.011);
    std::cout << color.pass_fail(item_passed, "%g", 9.99) << "\n";
    all_passed = all_passed && item_passed;

    std::cout << "To within 0.009 absolute (expect fail): ";
    item_passed = CheckItem::within_tolerance(9.99, 10, 0.009);
    std::cout << color.pass_fail(item_passed, "%g", 9.99) << "\n";
    all_passed = all_passed && (!item_passed);

    std::cout << "To within 0.0011 relative (expect pass): ";
    item_passed = CheckItem::within_tolerance(9.99, 10, 0.009, 0.0011);
    std::cout << color.pass_fail(item_passed, "%g", 9.99) << "\n";
    all_passed = all_passed && item_passed;

    std::cout << "To within 0.00095 relative (expect fail): ";
    item_passed = CheckItem::within_tolerance(9.99, 10, 0.009, 0.00095);
    std::cout << color.pass_fail(item_passed, "%g", 9.99) << "\n";
    all_passed = all_passed && (!item_passed);

    // Test the scalar is_within method.
    std::cout << "\n";
    std::cout << "Testing 9.99 versus bounds"
              << "\n";

    std::cout << "Between 9.98 and 10: (expect pass): ";
    item_passed = CheckItem::is_within(9.99, 9.98, 10.0);
    std::cout << color.pass_fail(item_passed, "%g", 9.99) << "\n";
    all_passed = all_passed && item_passed;

    std::cout << "Between 9.995 and 10: (expect fail): ";
    item_passed = CheckItem::is_within(9.99, 9.995, 10.0);
    std::cout << color.pass_fail(item_passed, "%g", 9.99) << "\n";
    all_passed = all_passed && (!item_passed);

    std::cout << "Between 9.98 and 9.985: (expect fail): ";
    item_passed = CheckItem::is_within(9.99, 9.98, 9.985);
    std::cout << color.pass_fail(item_passed, "%g", 9.99) << "\n";
    all_passed = all_passed && (!item_passed);

    // Test the vector within_tolerance methods.
    exp_vec[0] = -10;
    exp_vec[1] = 0;
    exp_vec[2] = 8;
    std::cout << "\n";

    std::cout << "Testing within_tolerance against [-10, 0, 8] "
              << "to within 0.8 absolute, 0.1 relative "
              << "\n";

    com_vec[0] = -9.91;
    com_vec[1] = 0.75;
    com_vec[2] = 7.19;

    std::cout << "Absolute only: Expect one failure @ vec[2]:         ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && (!item_passed) && out_vec[0] && out_vec[1] && (!out_vec[2]);

    std::cout << "Abs+relative: Expect one failure @ vec[2]:          ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, 0.1, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && (!item_passed) && out_vec[0] && out_vec[1] && (!out_vec[2]);

    com_vec[0] = -10.9;
    com_vec[1] = -0.81;
    com_vec[2] = 8.79;

    std::cout << "Absolute only: Expect failures @ vec[0] and vec[1]: ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && (!item_passed) && (!out_vec[0]) && (!out_vec[1]) && out_vec[2];

    std::cout << "Abs+relative: Expect failure @ vec[1]:              ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, 0.1, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && (!item_passed) && out_vec[0] && (!out_vec[1]) && out_vec[2];

    com_vec[0] = -10.99;
    com_vec[1] = -0.81;
    com_vec[2] = 7.21;

    std::cout << "Absolute only: Expect failures @ vec[0] and vec[1]: ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && (!item_passed) && (!out_vec[0]) && (!out_vec[1]) && out_vec[2];

    std::cout << "Abs+relative: Expect failure @ vec[1]:              ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, 0.1, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && (!item_passed) && out_vec[0] && (!out_vec[1]) && out_vec[2];

    com_vec[0] = -10.79;
    com_vec[1] = -0.79;

    std::cout << "Absolute only: Expect no failures                   ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && item_passed && out_vec[0] && out_vec[1] && out_vec[2];

    std::cout << "Abs+relative: Expect no failures                    ";
    item_passed = CheckItem::within_tolerance(com_vec, exp_vec, 0.8, 0.1, out_vec);
    std::cout << color.pass_fail(out_vec, "%6g", 1, 2, com_vec) << "\n";
    all_passed = all_passed && item_passed && out_vec[0] && out_vec[1] && out_vec[2];

    std::cout << "\n"
              << "Matrix tests of "
              << "within_tolerance, is_within, and is_identity\n"
              << "Expected matrix is the identity matrix.\n"
              << "Test matrix is\n"
              << "   0.998  -0.0001   1e-05\n"
              << "  0.0001  1.0009  -0.00049\n"
              << "  0.0001  0.00049  0.9992\n";

    Matrix3x3::identity(exp_mat);
    Matrix3x3::identity(com_mat);
    com_mat[0][0] = 1.0 - 2e-3;
    com_mat[1][1] = 1.0 + 0.9e-3;
    com_mat[2][2] = 1.0 - 0.8e-3;
    com_mat[0][1] = -1e-4;
    com_mat[0][2] = 1e-5;
    com_mat[1][0] = 1e-4;
    com_mat[1][2] = -4.9e-4;
    com_mat[2][0] = 1e-4;
    com_mat[2][1] = 4.9e-4;

    std::cout << "within_tolerance, "
              << "threshold = 5e-4 absolute\n"
              << "Expect three failures:\n";
    item_passed = CheckItem::within_tolerance(com_mat, exp_mat, 5e-4, out_mat);
    std::cout << color.pass_fail(out_mat, "%6g", 2, 2, com_mat) << "\n";
    all_passed = all_passed && (!item_passed) && (!out_mat[0][0]) && out_mat[0][1] && out_mat[0][2] && out_mat[1][0] &&
                 (!out_mat[1][1]) && out_mat[1][2] && out_mat[2][0] && out_mat[2][1] && (!out_mat[2][2]);

    std::cout << "within_tolerance, "
              << "thresholds = 5e-4 absolute, 1e-3 relative\n"
              << "Expect one failure:\n";
    item_passed = CheckItem::within_tolerance(com_mat, exp_mat, 5e-4, 1e-3, out_mat);
    std::cout << color.pass_fail(out_mat, "%6g", 2, 2, com_mat) << "\n";
    all_passed = all_passed && (!item_passed) && (!out_mat[0][0]) && out_mat[0][1] && out_mat[0][2] && out_mat[1][0] &&
                 out_mat[1][1] && out_mat[1][2] && out_mat[2][0] && out_mat[2][1] && out_mat[2][2];

    std::cout << "is_identity, "
              << "thresholds = 1e-3 diagonal, 5e-4 off-diagonal\n"
              << "Expect one failure:\n";
    item_passed = CheckItem::is_identity(com_mat, 1e-3, 5e-4, out_mat);
    std::cout << color.pass_fail(out_mat, "%6g", 2, 2, com_mat) << "\n";
    all_passed = all_passed && (!item_passed) && (!out_mat[0][0]) && out_mat[0][1] && out_mat[0][2] && out_mat[1][0] &&
                 out_mat[1][1] && out_mat[1][2] && out_mat[2][0] && out_mat[2][1] && out_mat[2][2];

    Matrix3x3::identity(com_mat);
    com_mat[0][0] -= 0.9e-3;
    com_mat[0][1] -= 1.1e-3;
    com_mat[0][2] += 1e-5;
    com_mat[1][0] += 1e-4;
    com_mat[1][1] += 2e-3;
    com_mat[1][2] -= 4.9e-4;
    com_mat[2][0] += 5.1e-4;
    com_mat[2][1] += 1e-4;
    com_mat[2][2] -= 1.1e-3;
    item_passed = CheckItem::within_tolerance(com_mat, exp_mat, 5e-4, 1e-3, out_mat);
    std::cout << "Expect four failures:"
              << "\n";
    std::cout << color.pass_fail(out_mat, "%6g", 2, 2, com_mat) << "\n";
    all_passed = all_passed && (!item_passed) && out_mat[0][0] && (!out_mat[0][1]) && out_mat[0][2] && out_mat[1][0] &&
                 (!out_mat[1][1]) && out_mat[1][2] && (!out_mat[2][0]) && out_mat[2][1] && (!out_mat[2][2]);

    // Summarize results.
    if(all_passed)
    {
        std::cout << color.pass_fail(true, "All tests passed.") << "\n";
        return 0;
    }
    else
    {
        std::cout << color.pass_fail(false, "One or more tests failed.") << "\n";
        return 1;
    }
}
