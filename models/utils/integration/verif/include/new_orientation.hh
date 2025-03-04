//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
/*******************************************************************************

Purpose:
  (Define the class used to specify the orientiation of one reference frame
   with respect to another. There are many competing charts on the rotation
   group. This class provides methods for converting among Euler rotations,
   transformation matrices, left transformation quaternions, and eigen
   rotations.)

Reference:
  ((TBS))

Library dependencies:
   ((../src/new_orientation.cc))



*******************************************************************************/

#ifndef JEOD_NEW_ORIENTATION_HH
#define JEOD_NEW_ORIENTATION_HH

// Jeod includes
#include "utils/quaternion/include/quat.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Define macros
#include "utils/math/include/macro_def.hh"

//! Namespace jeod
namespace jeod
{

class NewOrientation
{
    JEOD_MAKE_SIM_INTERFACES(jeod, NewOrientation)

public:
    enum DataSource
    {
        InputNone = -1,         // No source specified
        InputMatrix = 0,        // Transformation matrices supplied by user
        InputQuaternion = 1,    // Quaternions supplied by user
        InputEigenRotation = 2, // Single axis rotation supplied by user
        InputEulerRotation = 3  // Euler sequence and angles supplied by user
    };

    enum EulerSequence
    {
        NoSequence = -1, // No sequence specified

        RollPitchYaw = 0, // Roll Pitch Yaw
        RollYawPitch = 1, // Roll Yaw Pitch
        PitchYawRoll = 2, // Pitch Yaw Roll
        PitchRollYaw = 3, // Pitch Roll Yaw
        YawRollPitch = 4, // Yaw Roll Pitch
        YawPitchRoll = 5, // Yaw Pitch Roll

        EulerXYZ = 0, // Roll Pitch Yaw
        EulerXZY = 1, // Roll Yaw Pitch
        EulerYZX = 2, // Pitch Yaw Roll
        EulerYXZ = 3, // Pitch Roll Yaw
        EulerZXY = 4, // Yaw Roll Pitch
        EulerZYX = 5, // Yaw Pitch Roll

        EulerXYX = 6,  // Roll Pitch Roll
        EulerXZX = 7,  // Roll Yaw Roll
        EulerYXY = 8,  // Pitch Roll Pitch
        EulerYZY = 9,  // Pitch Yaw Pitch
        EulerZXZ = 10, // Yaw Roll Yaw
        EulerZYZ = 11  // Yaw Pitch Yaw
    };

public:
    NewOrientation() = default;
    virtual ~NewOrientation() = default;

    // Initializer
    virtual void initialize();

    // Update and access methods
    virtual void set_quaternion(const Quaternion & quat);

    virtual void get_quaternion(Quaternion & quat);

    virtual void set_matrix(const double trans[3][3]);

    virtual void get_matrix(double trans[3][3]);

    virtual void set_eigen_rotation(double eigen_angle, const double eigen_axis[3]);

    virtual void get_eigen_rotation(double & eigen_angle, double eigen_axis[3]);

    virtual void set_euler_angles(EulerSequence sequence, const double angles[3]);

    virtual void get_euler_angles(EulerSequence sequence, double angles[3]);

protected:
    void compute_quaternion_from_euler_angles();

    // Member data.
protected:
    DataSource data_source{InputNone}; /* trick_units(--) @n
      Orientation data source -- specifies whether the user has provided as
      input an Euler rotation, a transformation matrix, or a left transformation
      quaternion. */

    bool initialized_{}; /* trick_units(--) @n
       True if instance has been initialized. */

    bool have_quaternion_{}; /* trick_units(--) @n
       True if quaternion has been set/computed. */

    bool have_transformation_{}; /* trick_units(--) @n
       True if transformation matrix has been set/computed. */

    bool have_eigen_rotation_{}; /* trick_units(--) @n
       True if eigen rotation has been set/computed. */

    bool have_euler_angles_{}; /* trick_units(--) @n
       True if an Euler rotation has been set/computed. */

    EulerSequence euler_sequence{NoSequence}; /* trick_units(--) @n
       The Euler rotation sequence corresponding to euler_angles. */

    Quaternion quat; /* trick_units(--) @n
       Left transformation unit quaternion. */

    double eigen_angle{}; /* trick_units(rad) @n
       Single axis rotation angle. */

    double eigen_axis[3]{}; /* trick_units(--) @n
       Single axis rotation axis unit vector. */

    double euler_angles[3]{}; /* trick_units(rad) @n
        Euler angles corresponding to rotation sequence euler_sequence.
        The elements are stored in the order specified by that sequence. */

    double trans[3][3]{IDENTITY_3X3}; /* trick_units(--) @n
       Transformation matrix. */
};

} // namespace jeod

#include "utils/math/include/macro_undef.hh"

#endif
