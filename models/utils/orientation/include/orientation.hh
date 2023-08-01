//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Orientation
 * @{
 *
 * @file models/utils/orientation/include/orientation.hh
 * Define the Orientation class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
   ((../src/orientation.cc))

 

*******************************************************************************/


#ifndef JEOD_ORIENTATION_HH
#define JEOD_ORIENTATION_HH

// Jeod includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/quaternion/include/quat.hh"


//! Namespace jeod
namespace jeod {

/**
 * Specifies the orientiation of one reference frame with respect to another.
 * There are many competing charts on the rotation group.
 * This class provides means for representing rotations as Euler rotations,
 * transformation matrices, left transformation quaternions, and eigen
 * rotations. The class also provides mechanisms for converting these
 * representations into the alternative representations.
 */
class Orientation {

 JEOD_MAKE_SIM_INTERFACES(Orientation)

 // Type definitions
 public:

   /**
    * Specifies which representation has been input by the user.
    */
   enum DataSource {
      InputNone           = -1,  ///< No source specified
      InputMatrix         =  0,  ///< Transformation matrices supplied by user
      InputQuaternion     =  1,  ///< Quaternions supplied by user
      InputEigenRotation  =  2,  ///< Single axis rotation supplied by user
      InputEulerRotation  =  3   ///< Euler sequence and angles supplied by user
   };


   /**
    * Identifies which type of Euler sequence has been specified.
    */
   enum EulerSequence {
      NoSequence   = -1,  ///< No sequence specified

      // Sequences named by axes
      EulerXYZ       =  0,  ///< XYX sequence (roll pitch yaw)
      EulerXZY       =  1,  ///< XZY sequence (roll yaw pitch)
      EulerYZX       =  2,  ///< YZX sequence (pitch yaw roll)
      EulerYXZ       =  3,  ///< YXZ sequence (pitch roll yaw)
      EulerZXY       =  4,  ///< ZXY sequence (yaw roll pitch)
      EulerZYX       =  5,  ///< ZYX sequence (yaw pitch roll)

      // Astronomical Euler sequences, again named by axes
      EulerXYX       =  6,  ///< XYX sequence
      EulerXZX       =  7,  ///< XZX sequence
      EulerYZY       =  8,  ///< YZY sequence
      EulerYXY       =  9,  ///< YXY sequence
      EulerZXZ       = 10,  ///< The canonical ZXZ Euler sequence
      EulerZYZ       = 11,  ///< ZYZ sequence

      // Trick names as aliases for the first aerodynamic sequences
      Roll_Pitch_Yaw =  0,  ///< XYX sequence (roll pitch yaw)
      Roll_Yaw_Pitch =  1,  ///< XZY sequence (roll yaw pitch)
      Pitch_Yaw_Roll =  2,  ///< YZX sequence (pitch yaw roll)
      Pitch_Roll_Yaw =  3,  ///< YXZ sequence (pitch roll yaw)
      Yaw_Roll_Pitch =  4,  ///< ZXY sequence (yaw roll pitch)
      Yaw_Pitch_Roll =  5,  ///< ZYX sequence (yaw pitch roll)

      // Trick names without the underscores
      RollPitchYaw   =  0,  ///< XYX sequence (roll pitch yaw)
      RollYawPitch   =  1,  ///< XZY sequence (roll yaw pitch)
      PitchYawRoll   =  2,  ///< YZX sequence (pitch yaw roll)
      PitchRollYaw   =  3,  ///< YXZ sequence (pitch roll yaw)
      YawRollPitch   =  4,  ///< ZXY sequence (yaw roll pitch)
      YawPitchRoll   =  5   ///< ZYX sequence (yaw pitch roll)
   };

 // Static data.
 protected:
   /**
    * Threshold for detecting gimbal lock in compute_euler_angles_from_matrix.
    */
  static double gimbal_lock_threshold; //!< trick_units(--)

 // Static functions.
 public:

   static void compute_quaternion_from_euler_angles (
      EulerSequence sequence, const double angles[3], Quaternion & quat);

   static void compute_matrix_from_euler_angles (
      EulerSequence sequence, const double angles[3], double trans[3][3]);

   static void compute_euler_angles_from_matrix (
      const double trans[3][3], EulerSequence sequence, double angles[3]);

   static void compute_matrix_from_eigen_rotation (
      double eigen_angle, const double eigen_axis[3], double trans[3][3]);

   static void compute_eigen_rotation_from_matrix (
      const double trans[3][3], double * eigen_angle, double eigen_axis[3]);


 // Member functions.
 public:
   // Default constructor
   Orientation (void);

   // Non-default constructors
   explicit Orientation (const double trans_in[3][3]);
   explicit Orientation (const Quaternion & quat_in);
   explicit Orientation (double eigen_angle_in, const double eigen_axis[3]);
   explicit Orientation (EulerSequence sequence_in, const double angles_in[3]);

   // Destructor
   virtual ~Orientation (void);

   // Copy constructor and assignment operator:
   // These are not declared; the defaults supplied by the compiler
   // compiler work just fine in this case.


   // Clear knowledge of data contained in the object.
   virtual void reset (void);


   // Methods that compute a desired set of products.
   // NOTE WELL: The JEOD team reserves the right to protect these methods
   // (with advanced notice) in some future release.
   virtual void compute_transform (void);
   virtual void compute_quaternion (void);
   virtual void compute_eigen_rotation (void);
   virtual void compute_euler_angles (void);

   virtual void compute_all_products (void);
   virtual void compute_transformation_and_quaternion (void);


   // Setters and getters
   // Note that the getters are not const; some of them will modify
   // the instance.
   void set_quaternion (const Quaternion &quat);
   void get_quaternion (Quaternion &quat);

   void set_transform (const double trans[3][3]);
   void get_transform (double trans[3][3]);

   void set_eigen_rotation (double eigen_angle, const double eigen_axis[3]);
   void get_eigen_rotation (double * eigen_angle, double eigen_axis[3]);

   void set_euler_angles (EulerSequence sequence, const double angles[3]);
   void set_euler_angles (const double angles[3]);
   void get_euler_angles (EulerSequence * sequence, double angles[3]);
   void get_euler_angles (double angles[3]);

   EulerSequence get_euler_sequence (void);
   void set_euler_sequence (EulerSequence sequence);
   void clear_euler_sequence (void);


 protected:
   void mark_input_as_available ();

   void compute_quaternion_from_euler_angles (void);
   void compute_matrix_from_euler_angles (void);
   void compute_euler_angles_from_matrix (void);
   void compute_matrix_from_eigen_rotation (void);
   void compute_eigen_rotation_from_matrix (void);


 // Member data.
 public:
   /**
    * Orientation data source -- specifies whether the user has provided as
    * input an Euler rotation, a transformation matrix, or a left transformation
    * quaternion.
    */
   DataSource data_source;  //!< trick_units(--)

   /**
    * The Euler rotation sequence corresponding to euler_angles.
    */
   EulerSequence euler_sequence; //!< trick_units(--)

   /**
    * Euler angles corresponding to rotation sequence euler_sequence.
    * The elements are stored in the order specified by that sequence.
    */
   double euler_angles[3];//!< trick_units(rad)

   /**
    * Transformation matrix.
    */
   double trans[3][3]; //!< trick_units(--)

   /**
    * Left transformation unit quaternion.
    */
   Quaternion quat; //!< trick_units(--)

   /**
    * Single axis rotation angle.
    */
   double eigen_angle; //!< trick_units(rad)

   /**
    * Single axis rotation axis unit vector.
    */
   double eigen_axis[3]; //!< trick_units(--)

 protected:

   /**
    * True if transformation matrix has been set/computed.
    */
   bool have_transformation_; //!< trick_units(--)

   /**
    * True if quaternion has been set/computed.
    */
   bool have_quaternion_; //!< trick_units(--)

   /**
    * True if eigen rotation has been set/computed.
    */
   bool have_eigen_rotation_; //!< trick_units(--)

   /**
    * True if an Euler rotation has been set/computed.
    */
   bool have_euler_angles_; //!< trick_units(--)

};


/**
 * Compute the left transformation quaternion that corresponds to the
 * provided Euler rotation sequence.
 */
inline void
Orientation::compute_quaternion_from_euler_angles (
   void)
{
   compute_quaternion_from_euler_angles (euler_sequence, euler_angles, quat);

   return;
}


/**
 * Compute the transformation matrix that corresponds to the
 * provided Euler rotation sequence.
 */
inline void
Orientation::compute_matrix_from_euler_angles (
   void)
{
   compute_matrix_from_euler_angles (euler_sequence, euler_angles, trans);

   return;
}


/**
 * Compute an Euler rotation sequence that corresponds to the
 * provided transformation matrix.
 */
inline void
Orientation::compute_euler_angles_from_matrix (
   void)
{
   compute_euler_angles_from_matrix (trans, euler_sequence, euler_angles);

   return;
}


/**
 * Compute the transformation matrix that corresponds to the
 * provided eigen rotation.
 */
inline void
Orientation::compute_matrix_from_eigen_rotation (
   void)
{
   compute_matrix_from_eigen_rotation (eigen_angle, eigen_axis, trans);

   return;
}


/**
 * Compute a eigen rotation that corresponds to the
 * provided transformation matrix.
 */
inline void
Orientation::compute_eigen_rotation_from_matrix (
   void)
{
   compute_eigen_rotation_from_matrix (trans, &eigen_angle, eigen_axis);

   return;
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
