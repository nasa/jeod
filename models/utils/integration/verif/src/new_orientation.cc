/*******************************************************************************

Purpose:
  (Define methods for the NewOrientation class.)

Reference:
  ((TBS))

Library dependencies:
  ((new_orientation.o)
   (orientation_messages.o)
   (utils/message/message_handler.o)
   (utils/quaternion/quat.o)
   (utils/quaternion/quat_from_mat.o)
   (utils/quaternion/quat_norm.o)
   (utils/quaternion/quat_to_eigenrot.o)
   (utils/quaternion/quat_to_mat.o))

 
*******************************************************************************/


// System includes
#include <cmath>

// Jeod includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/new_orientation.hh"
#include "../include/random_orientation.hh"
#include "../include/orientation_messages.hh"



//! Namespace jeod
namespace jeod {

/*******************************************************************************
 Function: NewOrientation::NewOrientation
 Purpose: (Construct an NewOrientation instance.)
*******************************************************************************/
NewOrientation::NewOrientation (      // Return: -- N/A (constructor)
   void)
:
   data_source(InputNone),
   initialized_(false),
   have_quaternion_(false),
   have_transformation_(false),
   have_eigen_rotation_(false),
   have_euler_angles_(false),
   euler_sequence(NoSequence),
   quat(),
   eigen_angle(0.0)
{
   Vector3::initialize (eigen_axis);
   Vector3::initialize (euler_angles);
   Matrix3x3::identity (trans);

   return;
}


/*******************************************************************************
 Function: NewOrientation::initialize
 Purpose: (Initialize the instance from input.)
*******************************************************************************/
void
NewOrientation::initialize (   // Return: -- Void
   void)
{
   initialized_ =
   have_quaternion_ =
   have_transformation_ =
   have_eigen_rotation_ =
   have_euler_angles_ = false;


   switch (data_source) {

   case InputMatrix:
      have_transformation_ = have_quaternion_ = true;
      quat.left_quat_from_transformation (trans);
      break;

   case InputQuaternion:
      have_quaternion_ = true;
      break;

   case InputEigenRotation:
      have_eigen_rotation_ = have_quaternion_ = true;
      quat.left_quat_from_eigen_rotation (eigen_angle, eigen_axis);
      break;

   case InputEulerRotation: {
      have_euler_angles_ = have_quaternion_ = true;
      compute_quaternion_from_euler_angles ();
      break;
   }

   case InputNone:
   default:
      MessageHandler::fail (__FILE__, __LINE__,
                            OrientationMessages::invalid_enum,
                            "Invalid data_source value=%d",
                            static_cast<int> (data_source));
      break;
   }

   initialized_ = true;

   return;
}


/*******************************************************************************
 Function: NewOrientation::set_quaternion
 Purpose: (Reset the instance with a new quaternion.)
*******************************************************************************/
void
NewOrientation::set_quaternion (   // Return: -- Void
   const Quaternion & quat_in)
{
   initialized_ = have_quaternion_ = true;
   have_transformation_ = have_eigen_rotation_ = have_euler_angles_ = false;

   quat = quat_in;

   return;
}


/*******************************************************************************
 Function: NewOrientation::get_quaternion
 Purpose: (Accessor for the left transformation quaternion.)
*******************************************************************************/
void
NewOrientation::get_quaternion (   // Return: -- Void
   Quaternion & quat_out)
{
   if (! initialized_) {
      initialize ();
   }

   if (! have_quaternion_) {
      MessageHandler::fail (__FILE__, __LINE__,
                            OrientationMessages::invalid_request,
                            "Quaternion is not available.");
   }

   quat_out = quat;

   return;
}


/*******************************************************************************
 Function: NewOrientation::set_matrix
 Purpose: (Reset the instance with a new matrix.)
*******************************************************************************/
void
NewOrientation::set_matrix (   // Return: -- Void
   const double trans_in[3][3])
{
   initialized_ = have_transformation_ = have_quaternion_ = true;

   Matrix3x3::copy (trans_in, trans);
   quat.left_quat_from_transformation (trans);

   have_eigen_rotation_ = have_euler_angles_ = false;

   return;
}


/*******************************************************************************
 Function: NewOrientation::get_matrix
 Purpose: (Accessor for the transformation matrix.)
*******************************************************************************/
void
NewOrientation::get_matrix (   // Return: -- Void
   double trans_out[3][3])
{
   if (! initialized_) {
      initialize ();
   }

   if (! have_transformation_) {
      if (! have_quaternion_) {
         MessageHandler::fail (__FILE__, __LINE__,
                               OrientationMessages::invalid_request,
                               "Transformation matrix is not available.");
      }
      quat.left_quat_to_transformation (trans);
      have_transformation_ = true;
   }

   Matrix3x3::copy (trans, trans_out);

   return;
}


/*******************************************************************************
 Function: NewOrientation::set_eigen_rotation
 Purpose: (Reset the instance with a new eigen-rotation.)
*******************************************************************************/
void
NewOrientation::set_eigen_rotation (   // Return: -- Void
   double eigen_angle_in,
   const double eigen_axis_in[3])
{
   initialized_ = have_eigen_rotation_ = have_quaternion_ = true;

   eigen_angle = eigen_angle_in;
   Vector3::copy (eigen_axis_in, eigen_axis);

   quat.left_quat_from_eigen_rotation (eigen_angle, eigen_axis);

   have_transformation_ = have_euler_angles_ = false;

   return;
}


/*******************************************************************************
 Function: NewOrientation::get_eigen_rotation
 Purpose: (Accessor for the eigen rotation.)
*******************************************************************************/
void
NewOrientation::get_eigen_rotation (   // Return: -- Void
   double &eigen_angle_out,
   double eigen_axis_out[3])
{
   if (! initialized_) {
      initialize ();
   }

   if (! have_eigen_rotation_) {
      if (! have_quaternion_) {
         MessageHandler::fail (__FILE__, __LINE__,
                               OrientationMessages::invalid_request,
                               "Eigen rotation is not available.");
      }
      quat.left_quat_to_eigen_rotation (&eigen_angle, eigen_axis);
      have_eigen_rotation_ = true;
   }

   eigen_angle_out = eigen_angle;
   Vector3::copy (eigen_axis, eigen_axis_out);

   return;
}


/*******************************************************************************
 Function: NewOrientation::set_euler_angles
 Purpose: (Reset the instance with a new Euler rotation.)
*******************************************************************************/
void
NewOrientation::set_euler_angles (   // Return: -- Void
   EulerSequence sequence,
   const double angles[3])
{
   if ((static_cast <int>(euler_sequence) < 1) ||
       (static_cast <int>(euler_sequence) > 12)) {
      MessageHandler::fail (__FILE__, __LINE__,
                            OrientationMessages::invalid_enum,
                            "Invalid euler_sequence value=%d",
                            static_cast<int> (euler_sequence));
   }

   initialized_ = have_euler_angles_ = have_quaternion_ = true;

   euler_sequence = sequence;
   Vector3::copy (angles, euler_angles);

   compute_quaternion_from_euler_angles ();

   have_transformation_ = have_eigen_rotation_ = false;

   return;
}


/*******************************************************************************
 Function: NewOrientation::get_euler_angles
 Purpose: (Accessor for the Euler sequence.)
*******************************************************************************/
void
NewOrientation::get_euler_angles (   // Return: -- Void
   EulerSequence sequence __attribute__ ((unused)) ,
   double angles[3] __attribute__ ((unused)) )
{
   if (! initialized_) {
      initialize ();
   }

   MessageHandler::fail (__FILE__, __LINE__,
                         OrientationMessages::invalid_request,
                         "Method not implemented");

   return;
}


/*******************************************************************************
 Function: NewOrientation::compute_quaternion_from_euler_angles
 Purpose: (Compute the left transformation quaternion from the Euler sequence.)
*******************************************************************************/
void
NewOrientation::compute_quaternion_from_euler_angles (void)
{
   static int axes[12][3] = {
      { 0, 1, 2 },
      { 0, 2, 1 },
      { 1, 2, 0 },
      { 1, 0, 2 },
      { 2, 0, 1 },
      { 2, 1, 0 },
      { 0, 1, 0 },
      { 0, 2, 0 },
      { 1, 0, 1 },
      { 1, 2, 1 },
      { 2, 0, 2 }
   };

   int set_index = static_cast <int>(euler_sequence) - 1;
   Quaternion q[3], q21;

   for (int ii = 0; ii < 3; ii++) {
      double htheta = 0.5*euler_angles[ii];
      double cosht = cos (htheta);
      double sinht = sin (htheta);
      q[ii].scalar = cosht;
      q[ii].vector[axes[set_index][ii]] = -sinht;
   }

   q[2].multiply (q[1], q21);
   q21.multiply (q[0], quat);
   quat.normalize ();

   return;
}


/*******************************************************************************
 Function: VectorOrientation::VectorOrientation
 Purpose: (Constructor.)
*******************************************************************************/
VectorOrientation::VectorOrientation (
   bool init_from_vector)
:
   initialize_from_vector(init_from_vector)
{
   vector[0] = 0.0;
   vector[1] = 0.0;
   vector[2] = 0.0;
}


/*******************************************************************************
 Function: VectorOrientation::initialize
 Purpose: (Initialize a VectorOrientation.)
*******************************************************************************/
void
VectorOrientation::initialize (
   void)
{
   if (initialize_from_vector) {
      double ht, cosht, sinht, cosphi, sinphi, psi, cospsi, sinpsi;

      // Convert the input vector elements from to angles
      // Notes:
      //  - Ideally, elements 0 and 2 will be drawn from U[0,1) while element 1
      //    should be drawn from U[0,1]. (Rotation about -z is not representable
      //    if vector[1]] is drawn from U[0,1).)
      //  - There is no protection against elements being outside [0,1].
      //  - Converting vector[1] to cosine rather than angle leads to a uniform
      //    distribution over rotation space.
      ht     = M_PI * vector[0];
      cosphi = 2.0 * vector[1] - 1.0;
      psi    = 2.0 * M_PI * vector[2];

      // Extract sines and cosines of the angles.
      cosht = cos(ht);
      sinht = sin(ht);

      sinphi = sqrt(1.0 - cosphi*cosphi);

      cospsi = cos(psi);
      sinpsi = sin(psi);

      // Generate the corresponding quaternion.
      quat.scalar    =  cosht;
      quat.vector[0] = -sinht * sinphi * cospsi;
      quat.vector[1] = -sinht * sinphi * sinpsi;
      quat.vector[2] = -sinht * cosphi;

      data_source = InputQuaternion;
   }

   NewOrientation::initialize();
}


/*******************************************************************************
 Function: RandomOrientation::RandomOrientation
 Purpose: (Constructor.)
*******************************************************************************/
RandomOrientation::RandomOrientation (
   RandomSeedGenerator & seed_gen,
   bool random_orientation)
:
   VectorOrientation(),
   generate_random_vector(random_orientation),
   skip_count(0),
   rvec (seed_gen)
{
}


/*******************************************************************************
 Function: RandomOrientation::RandomOrientation
 Purpose: (Initialize a RandomOrientation.)
*******************************************************************************/
void
RandomOrientation::initialize (
   void)
{
   if (generate_random_vector) {
      for (unsigned int ii = 0; ii < skip_count; ++ii) {
         rvec.random_vector (vector);
      }
      rvec.random_vector (vector);
      initialize_from_vector = true;
   }
   VectorOrientation::initialize();
}
} // End JEOD namespace
