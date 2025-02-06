/*******************************************************************************

Purpose:
  (Test the static methods that pertain to converting to and from Euler angles.
   There are three such methods:
    - Orientation::compute_quaternion_from_euler_angles,
    - Orientation::compute_matrix_from_euler_angles, and
    - Orientation::compute_euler_angles_from_matrix.
   Note that the methods under test are the static methods that take arguments.
   The overloaded non-static methods with the same name but different signature
   (no arguments) are not the subject of this test.

   The test covers
    - Each of the twelve Euler sequence configurations,
    - Non-gimbal lock situations,
    - Pure gimbal lock situations,
    - Near gimbal lock situations, and
    - Covers the space of the rotations.)

*******************************************************************************/


#include <cmath>

#include "trick_utils/math/include/trick_math.h"

#include "utils/orientation/include/orientation.hh"
#include "utils/math/include/matrix3x3.hh"

#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/color_text.hh"
#include "test_harness/include/test_sim_interface.hh"

using namespace jeod;

#define STEPS_PER_2PI 32

unsigned int ntests = 0;
unsigned int npassed = 0;
int verbose = 0;
ColorText color;

inline double dmax (double a, double b) {
   return a > b ? a : b;
}

double test_theta (
   double theta,
   unsigned int test_id,
   double threshold);

double run_test (
   unsigned int test_id,
   const double angles[3],
   unsigned int ibeg,
   unsigned int iend,
   double threshold);


int main (
   int argc,
   char ** argv)
{
   int return_code = 1;

   TestSimInterface sim_interface;
   CmdlineParser cmdline_parser;

   unsigned int test_id = 0;
   unsigned int prev_tests = 0;
   unsigned int prev_passed = 0;
   double log_10 = std::log (10.0);
   double err;
   double threshold;


   cmdline_parser.add_counter ("verbose", &verbose);
   cmdline_parser.parse (argc, argv);


   prev_tests = ntests;
   prev_passed = npassed;
   threshold = 1e-15;
   printf ("Testing %s (threshold=%g)\n",
           "rotations far from gimbal lock", threshold);
   for (unsigned int itheta = 1; itheta <= 8; ++itheta) {
      double theta = static_cast<double>(itheta) * M_PI / 16.0;
      ++test_id;
      err = test_theta (theta, test_id, threshold);
   }

   printf ("Passed %d out of %d tests\n\n",
           npassed-prev_passed, ntests-prev_tests);


   prev_tests = ntests;
   prev_passed = npassed;
   threshold = 1e-15;
   printf ("Testing %s (threshold=%g)\n",
           "pure gimbal lock cases", threshold);
   {
      double theta = 0.0;
      ++test_id;
      err = test_theta (theta, test_id, threshold);
   }

   printf ("Passed %d out of %d tests\n\n",
           npassed-prev_passed, ntests-prev_tests);


   prev_tests = ntests;
   prev_passed = npassed;
   threshold = 3e-13;
   printf ("Testing %s (threshold=%g)\n",
           "rotations within 0.02 mas of gimbal lock", threshold);
   for (int itheta = -38; itheta < -19; ++itheta) {
      double theta = exp(((double)itheta)*0.5*log_10);
      ++test_id;
      err = test_theta (theta, test_id, threshold);
   }

   printf ("Passed %d out of %d tests\n\n",
           npassed-prev_passed, ntests-prev_tests);


   prev_tests = ntests;
   prev_passed = npassed;
   threshold = 1e-15;
   printf ("Testing %s (threshold=%g)\n",
           "rotations between 0.02 mas and 652 mas of gimbal lock", threshold);
   for (int itheta = -68; itheta < -43; ++itheta) {
      double theta = exp(((double)itheta)*0.125*log_10);
      ++test_id;
      err = test_theta (theta, test_id, threshold);
   }
   printf ("Passed %d out of %d tests\n\n",
           npassed-prev_passed, ntests-prev_tests);


   prev_tests = ntests;
   prev_passed = npassed;
   threshold = 1e-15;
   printf ("Testing %s (threshold=%g)\n",
           "rotations between 652 mas and 5.7 degrees of gimbal lock",
           threshold);
   for (int itheta = -11; itheta < -1; ++itheta) {
      double theta = exp(((double)itheta)*0.5*log_10);
      ++test_id;
      err = test_theta (theta, test_id, threshold);
   }
   printf ("Passed %d out of %d tests\n\n",
           npassed-prev_passed, ntests-prev_tests);


   if (npassed == ntests) {
      printf ("Summary: Passed all %d tests\n\n", ntests);
      return_code = 0;
   }
   else {
      printf ("Summary: Passed %d out of %d tests\n\n", npassed, ntests);
      return_code = 1;
   }

   return return_code;
}


double
test_theta (
   double theta,
   unsigned int test_id,
   double threshold)
{
   double max_err = 0.0;
   double scale = 2.0 * M_PI / static_cast<double>(STEPS_PER_2PI);
   unsigned int seq_cases[2][2] = {{0, 6}, {6, 12}};
   double theta_cases[2][2] = {{theta - 0.5*M_PI, 0.5*M_PI - theta},
                               {theta, M_PI - theta}};
   double test_angles[3];
   double err;
   unsigned int ntheta;

   if (std::fabs(theta_cases[1][0] - theta_cases[1][1]) < 1e-15) {
      ntheta = 1;
   }
   else {
      ntheta = 2;
   }

   for (unsigned int itheta = 0; itheta < ntheta; ++itheta) {
      for (unsigned int iphi = 0; iphi < STEPS_PER_2PI; ++iphi) {
         test_angles[0] = static_cast<double>(iphi) * scale;
         for (int ipsi = 0; ipsi < STEPS_PER_2PI; ++ipsi) {
            test_angles[2] = static_cast<double>(ipsi) * scale;
            for (unsigned int seq_set = 0; seq_set < 2; ++seq_set) {
               unsigned int ibeg = seq_cases[seq_set][0];
               unsigned int iend = seq_cases[seq_set][1];
               test_angles[1] = theta_cases[seq_set][itheta];
               err = run_test (test_id, test_angles, ibeg, iend, threshold);
               max_err = dmax (err, max_err);
            }
         }
      }
   }
   if (verbose > 0) {
      printf ("%g %g\n", theta, max_err);
   }
   return max_err;
}


double
run_test (
   unsigned int test_id,
   const double angles[3],
   unsigned int ibeg,
   unsigned int iend,
   double threshold)
{
   double max_err = 0.0;

   for (unsigned int iseq = ibeg; iseq < iend; ++iseq) {
      Orientation::EulerSequence sequence = (Orientation::EulerSequence) iseq;
      double test_angles[3];
      Quaternion quat_from_model;
      double trans_from_model[3][3];
      double angles_from_model[3];
      double trans_from_trick[3][3];
      double err_matrix[3][3];
      Quaternion err_quat;
      double err_axes[3];
      double err_angle1, err_angle2, err_angle3;
      bool pass1, pass2, pass3, passed;

      Vector3::copy (angles, test_angles);


      Orientation::compute_quaternion_from_euler_angles (
         sequence, test_angles, quat_from_model);
      Orientation::compute_matrix_from_euler_angles (
         sequence, test_angles, trans_from_model);
      Orientation::compute_euler_angles_from_matrix (
         trans_from_model, sequence, angles_from_model);


      // Calculate the error between the matrix as calculated by Trick
      // and that calculated by this model.
      // Note that
      //  - Trick only handles the Tait-Bryan angles.
      //  - The tolerance is a fixed (and very low) threshold.
      if (sequence < 6) {
         euler_matrix (test_angles, trans_from_trick, 0, (Euler_Seq) iseq);
         Matrix3x3::product_right_transpose (
            trans_from_model, trans_from_trick, err_matrix);
         err_quat.left_quat_from_transformation (err_matrix);
         err_quat.left_quat_to_eigen_rotation (&err_angle1, err_axes);
         pass1 = (err_angle1 <= 1e-15);
      }
      else {
         err_angle1 = 0.0;
         pass1 = true;
      }


      // Calculate the error between the quaternion and matrix generated from
      // the euler rotation sequence.
      // Note that the tolerance for this is also a fixed (and low) threshold.
      err_quat.left_quat_from_transformation (trans_from_model);
      err_quat.eigen_compare (quat_from_model, &err_angle2, err_axes);
      pass2 = (err_angle2 <= 1e-15);


      // Calculate the error between the original and computed angles.
      Orientation::compute_quaternion_from_euler_angles (
         sequence, angles_from_model, err_quat);
      err_quat.eigen_compare (quat_from_model, &err_angle3, err_axes);
      pass3 = (err_angle3 <= threshold);


      // The test passes if all three angles are within tolerance.
      passed = pass1 && pass2 && pass3;

      // Report results.
      if ((verbose > 1) || !passed) {
         printf ("%4d %2d  %s %s %s\n",
                 test_id, iseq,
                 (sequence < 6 ?
                  color.pass_fail(pass1, "%9.2e", err_angle1).c_str() :
                  "         "),
                 color.pass_fail(pass2, "%9.2e", err_angle2).c_str(),
                 color.pass_fail(pass3, "%9.2e", err_angle3).c_str());
      }

      max_err = dmax (err_angle3, max_err);

      npassed += passed ? 1 : 0;
      ntests += 1;
   }

   return max_err;
}
