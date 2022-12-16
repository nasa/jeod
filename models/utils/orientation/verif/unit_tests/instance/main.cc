/*
Purpose:
  (Test the non-static interfaces to the Orientation class.
   The test driver is admittedly bizarre and twisted.
   The intent is to force coverage of all orientation.cc code except for
   the error handling.)
*/

#include <cmath>
#include <cstdio>
#include <stdlib.h>
#include "trick_utils/math/include/trick_math.h"

#include "utils/orientation/include/orientation.hh"
#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"

#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/color_text.hh"
#include "test_harness/include/test_sim_interface.hh"

using namespace jeod;

bool verbose;
ColorText color;
unsigned int tests_made = 0;
unsigned int tests_passed = 0;
const char * euler_sequence_name[12] = {
   "EulerXYZ", "EulerXZY", "EulerYZX", "EulerYXZ", "EulerZXY", "EulerZYX",
   "EulerXYX", "EulerXZX", "EulerYZY", "EulerYXY", "EulerZXZ", "EulerZYZ"};


class TestData {
 public:
   TestData ();
   TestData (
      const double trans_in[3][3],
      const Quaternion quat_in,
      const double eigen_rot_in,
      const double eigen_axis_in[3],
      const double euler_angles_in[12][3]);

   void set_data (
      const double trans_in[3][3],
      const Quaternion quat_in,
      const double eigen_rot_in,
      const double eigen_axis_in[3],
      const double euler_angles_in[12][3]);

   double trans[3][3];
   Quaternion quat;
   double eigen_rot;
   double eigen_axis[3];
   double euler_angles[12][3];
};


class TruthData : public TestData {
 public:
   TruthData ();
};


void test_getters (Orientation::DataSource, const TruthData&, Orientation&);
void test_contents (const TruthData&, Orientation&);
void get_euler_angles (Orientation &, double [12][3]);
void test_results (const TruthData&, const TestData&);
void update_results (bool);


void die (
   const char * msg)
{
   printf ("%s\n", msg);
   exit (1);
}


int
main (
   int argc,
   char * argv[])
{

   CmdlineParser cmdline_parser;

   TestSimInterface sim_interface;

   TruthData truth_data;
   unsigned int ncases = 0;


   cmdline_parser.add_switch ("verbose", &verbose);
   cmdline_parser.parse (argc, argv);


   // Test setters and getters.
   {
      if (verbose) {
         printf ("Testing Orientation::set_transform()\n");
      }
      Orientation orient;
      orient.set_transform (truth_data.trans);
      test_getters (Orientation::InputMatrix, truth_data, orient);
      ncases++;

      orient.reset();
      if (verbose) {
         printf ("Testing Orientation::set_transform() after reset()\n");
      }
      orient.set_transform (truth_data.trans);
      test_getters (Orientation::InputNone, truth_data, orient);
      ncases++;
   }

   {
      if (verbose) {
         printf ("Testing Orientation::set_quaternion()\n");
      }
      Orientation orient;
      orient.set_quaternion (truth_data.quat);
      test_getters (Orientation::InputQuaternion, truth_data, orient);
      ncases++;
   }

   {
      if (verbose) {
         printf ("Testing Orientation::set_eigen_rotation()\n");
      }
      Orientation orient;
      orient.set_eigen_rotation (truth_data.eigen_rot, truth_data.eigen_axis);
      test_getters (Orientation::InputEigenRotation, truth_data, orient);
      ncases++;
   }

   for (unsigned int iseq = 0; iseq < 12; ++iseq) {
      if (verbose) {
         printf ("Testing Orientation::set_euler_angles(), sequence=%s\n",
                 euler_sequence_name[iseq]);
      }
      Orientation orient;
      Orientation::EulerSequence sequence = (Orientation::EulerSequence) iseq;
      if (iseq < 6) {
         orient.set_euler_angles (sequence, truth_data.euler_angles[iseq]);
      }
      else {
         orient.set_euler_sequence (sequence);
         orient.set_euler_angles (truth_data.euler_angles[iseq]);
      }
      test_getters (Orientation::InputEulerRotation, truth_data, orient);
      ncases++;
   }


   // Test non-default constructors and getters.
   {
      if (verbose) {
         printf ("Testing Orientation constructor(matrix)\n");
      }
      Orientation orient (truth_data.trans);
      test_getters (Orientation::InputMatrix, truth_data, orient);
      ncases++;
   }

   {
      if (verbose) {
         printf ("Testing Orientation constructor(quaternion)\n");
      }
      Orientation orient (truth_data.quat);
      test_getters (Orientation::InputQuaternion, truth_data, orient);
      ncases++;
   }

   {
      if (verbose) {
         printf ("Testing Orientation constructor(eigen rotation)\n");
      }
      Orientation orient (truth_data.eigen_rot, truth_data.eigen_axis);
      test_getters (Orientation::InputEigenRotation, truth_data, orient);
      ncases++;
   }

   for (unsigned int iseq = 0; iseq < 12; ++iseq) {
      if (verbose) {
         printf ("Testing Orientation constructor(euler angles), sequence=%s\n",
                 euler_sequence_name[iseq]);
      }
      Orientation::EulerSequence sequence = (Orientation::EulerSequence) iseq;
      Orientation orient (sequence, truth_data.euler_angles[iseq]);
      test_getters (Orientation::InputEulerRotation, truth_data, orient);
      ncases++;
   }


   // Test poking and peeking.
   {
      if (verbose) {
         printf ("Testing data initialization (matrix)\n");
      }
      Orientation orient;
      Matrix3x3::copy (truth_data.trans, orient.trans);
      orient.data_source = Orientation::InputMatrix;
      test_contents (truth_data, orient);
      ncases++;
   }

   {
      if (verbose) {
         printf ("Testing data initialization (quaternion)\n");
      }
      Orientation orient;
      orient.quat = truth_data.quat;
      orient.data_source = Orientation::InputQuaternion;
      test_contents (truth_data, orient);
      ncases++;
   }

   {
      if (verbose) {
         printf ("Testing data initialization (eigen rotation)\n");
      }
      Orientation orient;
      orient.eigen_angle = truth_data.eigen_rot;
      Vector3::copy (truth_data.eigen_axis, orient.eigen_axis);
      orient.data_source = Orientation::InputEigenRotation;
      test_contents (truth_data, orient);
      ncases++;
   }

   for (unsigned int iseq = 0; iseq < 12; ++iseq) {
      if (verbose) {
         printf ("Testing data initialization (euler angles), sequence=%s\n",
                 euler_sequence_name[iseq]);
      }
      Orientation orient;
      Orientation::EulerSequence sequence = (Orientation::EulerSequence) iseq;
      orient.euler_sequence = sequence;
      Vector3::copy (truth_data.euler_angles[iseq], orient.euler_angles);
      orient.data_source = Orientation::InputEulerRotation;
      test_contents (truth_data, orient);
      ncases++;
   }


   printf ("Extracted %u items from %u cases; number failures = %u\n",
           tests_made, ncases, tests_made-tests_passed);
   return (tests_passed == tests_made) ? 0 : 1;
}


void
test_getters (
   Orientation::DataSource source,
   const TruthData & truth_data,
   Orientation & orient)
{
   double trans_out[3][3];
   Quaternion quat_out;
   double eigen_rot_out;
   double eigen_axis_out[3];
   double euler_angles_out[12][3];

   switch (source) {
   case Orientation::InputMatrix:
      orient.get_transform (trans_out);
      get_euler_angles (orient, euler_angles_out);
      orient.get_eigen_rotation (&eigen_rot_out, eigen_axis_out);
      orient.get_quaternion (quat_out);
      break;

   case Orientation::InputQuaternion:
      orient.get_quaternion (quat_out);
      orient.get_eigen_rotation (&eigen_rot_out, eigen_axis_out);
      get_euler_angles (orient, euler_angles_out);
      orient.get_transform (trans_out);
      break;

   case Orientation::InputEigenRotation:
      orient.get_eigen_rotation (&eigen_rot_out, eigen_axis_out);
      get_euler_angles (orient, euler_angles_out);
      orient.get_quaternion (quat_out);
      orient.get_transform (trans_out);
      break;

   case Orientation::InputEulerRotation: {
      Orientation::EulerSequence sequence;
      double euler_angles[3];
      if (orient.euler_sequence < Orientation::EulerXYX) {
         orient.get_euler_angles (&sequence, euler_angles);
      }
      else {
         sequence = orient.get_euler_sequence ();
         orient.get_euler_angles (euler_angles);
      }
      if ((sequence != orient.euler_sequence) ||
          (euler_angles[0] != orient.euler_angles[0]) ||
          (euler_angles[1] != orient.euler_angles[1]) ||
          (euler_angles[2] != orient.euler_angles[2])) {
         die ("Input mechanism error");
      }
      switch (static_cast <int> (orient.euler_sequence)) {
      case 0: case 1: case 2: case 3:
         orient.get_eigen_rotation (&eigen_rot_out, eigen_axis_out);
         orient.get_transform (trans_out);
         orient.get_quaternion (quat_out);
         get_euler_angles (orient, euler_angles_out);
         break;
      case 4: case 5: case 6: case 7:
         get_euler_angles (orient, euler_angles_out);
         orient.get_eigen_rotation (&eigen_rot_out, eigen_axis_out);
         orient.get_transform (trans_out);
         orient.get_quaternion (quat_out);
         break;
      case 8: case 9: case 10: case 11:
         orient.get_transform (trans_out);
         orient.get_quaternion (quat_out);
         orient.get_eigen_rotation (&eigen_rot_out, eigen_axis_out);
         get_euler_angles (orient, euler_angles_out);
         break;
      }
      break;
   }

   case Orientation::InputNone:
      orient.compute_all_products ();
      Matrix3x3::copy (orient.trans, trans_out);
      quat_out = orient.quat;
      eigen_rot_out = orient.eigen_angle;
      Vector3::copy (orient.eigen_axis, eigen_axis_out);
      get_euler_angles (orient, euler_angles_out);
      break;

   default:
      die ("Internal error");
   }


   TestData test_data (
      trans_out, quat_out, eigen_rot_out, eigen_axis_out, euler_angles_out);

   test_results (truth_data, test_data);
}


void
test_contents (
   const TruthData & truth_data,
   Orientation & orient)
{
   double trans_out[3][3];
   Quaternion quat_out;
   double eigen_rot_out;
   double eigen_axis_out[3];
   double euler_angles_out[12][3];

   switch (orient.data_source) {
   case Orientation::InputMatrix:
      orient.compute_transform ();
      orient.compute_eigen_rotation ();
      orient.compute_quaternion ();
      break;

   case Orientation::InputQuaternion:
      orient.compute_quaternion ();
      orient.compute_eigen_rotation ();
      orient.compute_transform ();
      break;

   case Orientation::InputEigenRotation:
      orient.compute_eigen_rotation ();
      orient.compute_transformation_and_quaternion();
      break;

   case Orientation::InputEulerRotation: {
      orient.compute_euler_angles ();
      orient.compute_transformation_and_quaternion();
      orient.compute_eigen_rotation ();
      break;
   }

   default:
      die ("Internal error");
   }

   Matrix3x3::copy (orient.trans, trans_out);
   quat_out = orient.quat;
   eigen_rot_out = orient.eigen_angle;
   Vector3::copy (orient.eigen_axis, eigen_axis_out);

   get_euler_angles (orient, euler_angles_out);


   TestData test_data (
      trans_out, quat_out, eigen_rot_out, eigen_axis_out, euler_angles_out);

   test_results (truth_data, test_data);
}


void
get_euler_angles (
   Orientation & orient,
   double euler_angles_out[12][3])
{
   for (unsigned int iseq = 0; iseq < 12; ++iseq) {
      Orientation::EulerSequence sequence = (Orientation::EulerSequence) iseq;
      orient.set_euler_sequence (sequence);
      orient.get_euler_angles (euler_angles_out[iseq]);
   }
}


void
test_results (
   const TruthData & truth_data,
   const TestData & test_data)
{
   double err_matrix[3][3];
   Quaternion err_quat;
   double err_angle, err_axes[3];
   double err_trace, err_vec[3];
   bool passed;

   // Compare the transformation matrices.
   Matrix3x3::product_right_transpose (
      truth_data.trans, test_data.trans, err_matrix);
   err_vec[0] = err_matrix[1][2] - err_matrix[2][1];
   err_vec[1] = err_matrix[2][0] - err_matrix[0][2];
   err_vec[2] = err_matrix[0][1] - err_matrix[1][0];
   err_angle = 0.5 * Vector3::vmag (err_vec);
   err_trace = err_matrix[0][0] + err_matrix[1][1] + err_matrix[2][2];

   passed = (err_trace > 3.0-1e-14) && (err_angle <= 1e-15);
   if (verbose || !passed) {
      printf ("  Transformation matrix test %s\n",
              passed ? "passed" : "failed");
   }
   update_results (passed);

   // Compare the quaternions.
   truth_data.quat.eigen_compare (test_data.quat, &err_angle, err_axes);
   passed = (err_angle <= 1e-15);
   if (verbose || !passed) {
      printf ("  Transformation quaternion test %s\n",
              passed ? "passed" : "failed");
   }
   update_results (passed);

   // Compare the eigen rotations.
   Vector3::cross (truth_data.eigen_axis, test_data.eigen_axis, err_vec);
   err_angle = truth_data.eigen_rot - test_data.eigen_rot;
   err_angle = std::sqrt (err_angle*err_angle + Vector3::vmagsq(err_vec));
   passed = (err_angle <= 1e-15);
   if (verbose || !passed) {
      printf ("  Eigen rotation test %s\n",
              passed ? "passed" : "failed");
   }
   update_results (passed);

   // Compare each of the Euler angles.
   for (unsigned int iseq = 0; iseq < 12; ++iseq) {
      double test_matrix[3][3];
      Orientation::compute_matrix_from_euler_angles (
         static_cast <Orientation::EulerSequence> (iseq),
         test_data.euler_angles[iseq],
         test_matrix);
      Vector3::diff (
         truth_data.euler_angles[iseq], test_data.euler_angles[iseq], err_vec);
      err_angle = Vector3::vmag (err_vec);
      passed = (err_angle <= 1e-15);
      if (verbose || !passed) {
         printf ("  Euler angle %s test %s\n",
                 euler_sequence_name[iseq],
                 passed ? "passed" : "failed");
      }
      update_results (passed);
   }
}


void
update_results (
   bool passed)
{
   tests_made++;
   if (passed) {
      tests_passed++;
   }
}


TestData::TestData (
   void)
{ }

TestData::TestData (
   const double trans_in[3][3],
   const Quaternion quat_in,
   const double eigen_rot_in,
   const double eigen_axis_in[3],
   const double euler_angles_in[12][3])
{
   set_data (trans_in, quat_in, eigen_rot_in, eigen_axis_in, euler_angles_in);
}


void
TestData::set_data (
   const double trans_in[3][3],
   const Quaternion quat_in,
   const double eigen_rot_in,
   const double eigen_axis_in[3],
   const double euler_angles_in[12][3])
{
   Matrix3x3::copy (trans_in, trans);
   quat = quat_in;
   eigen_rot = eigen_rot_in;
   Vector3::copy (eigen_axis_in, eigen_axis);
   for (unsigned int iseq = 0; iseq < 12; ++iseq) {
      Vector3::copy (euler_angles_in[iseq], euler_angles[iseq]);
   }
}


TruthData::TruthData (
   void)
{
   double half_pi = M_PI / 2.0;
   double root_3  = std::sqrt (3.0);
   double trans_in[3][3] = {{0,1,0},{0,0,1},{1,0,0}};
   Quaternion quat_in;
   double uhat[3] = {1.0/root_3, 1.0/root_3, 1.0/root_3};
   double euler_angles_in[12][3] = {
      {half_pi, half_pi, 0.0}, {half_pi, 0.0, half_pi},
      {half_pi, half_pi, 0.0}, {half_pi, 0.0, half_pi},
      {half_pi, half_pi, 0.0}, {half_pi, 0.0, half_pi},
      {half_pi, half_pi, 0.0}, {0.0, half_pi, half_pi},
      {half_pi, half_pi, 0.0}, {0.0, half_pi, half_pi},
      {half_pi, half_pi, 0.0}, {0.0, half_pi, half_pi}};

   quat_in.left_quat_from_eigen_rotation (M_PI * 2.0/3.0, uhat);
   set_data (trans_in, quat_in, M_PI*2.0/3.0, uhat, euler_angles_in);
}
