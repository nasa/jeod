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
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/include/quat.hh
 * Define the quaternion class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((../src/quat.cc)
   (../src/quat_norm.cc)
   (../src/quat_to_mat.cc)
   (../src/quat_from_mat.cc)
   (../src/quat_to_eigenrot.cc)
   (../src/quat_messages.cc))


*******************************************************************************/

#ifndef JEOD_QUATERNION_HH
#define JEOD_QUATERNION_HH

#include <cstdlib>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Implement quaternions to the extent needed to represent orientations.
 */
class Quaternion
{
    JEOD_MAKE_SIM_INTERFACES(jeod, Quaternion)

public:
    // Member data
    /**
     * The scalar, or real, part of the quaternion.
     */
    double scalar{1.0}; //!< trick_units(--)

    /**
     * The vectorial, or imaginary, part of the quaternion.
     */
    double vector[3]{}; //!< trick_units(--)

    // Member functions
    // Note: This class allocates no resources. The defaults for the
    // destructor, copy constructor, and assignment operator are correct.

    // Default constructor: Construct pure real unit quaternion
    Quaternion();

    // Non-default constructor: Construct pure real quaternion
    explicit Quaternion(const double s);

    // Non-default constructor: Construct from a scalar and a vector.
    Quaternion(const double s, const double v[3]);

    // Non-default constructor: Construct from a double[4] array.
    explicit Quaternion(const double arr[4]);

    // Non-default constructor: Construct quaternion as a left transformation
    // quaternion given a transformation matrix
    explicit Quaternion(const double T[3][3]);

    // Simple methods to make a pure zero, identity quaternion
    void set_to_zero();
    void make_identity();

#ifndef SWIG
    /**
     * Make a quaternion look like a double array.
     */
    operator double *()
    {
        return &scalar;
    }
#endif

    // Copy a quaternion to/from a double[4] array.
    void copy_to(double arr[4]) const;
    void copy_from(const double arr[4]);

    // Scale a quaternion by a real.
    void scale(const double scale);
    void scale(const double scale, Quaternion & quat) const;

    // Square of the norm of a quaternion
    double norm_sq() const;

    // Normalize a quaternion (scale to form a unit quaternion),
    // in place and with seperate output.
    void normalize();
    void normalize(Quaternion & quat) const;

    void normalize_integ();
    void normalize_integ(Quaternion & quat) const;
    static void normalize_integ(double arr[4]);

    // Conjugate a quaternion,
    // in place and with seperate output.
    void conjugate();
    void conjugate(Quaternion & quat) const;

    // Multiply a quaternion with another, various forms.
    void multiply(const Quaternion & quat, Quaternion & prod) const;
    void multiply(const Quaternion & quat);

    void conjugate_multiply(const Quaternion & quat, Quaternion & prod) const;
    void conjugate_multiply(const Quaternion & quat);

    void multiply_conjugate(const Quaternion & quat, Quaternion & prod) const;
    void multiply_conjugate(const Quaternion & quat);

    void multiply_left(const Quaternion & quat, Quaternion & prod) const;
    void multiply_left(const Quaternion & quat);

    void multiply_left_conjugate(const Quaternion & quat, Quaternion & prod) const;
    void multiply_left_conjugate(const Quaternion & quat);

    // Multiply a quaternion with a pure imaginary quaterion, various forms.
    void multiply_vector_left(const double vec[3], Quaternion & prod) const;

    void multiply_vector_right(const double vec[3], Quaternion & prod) const;

    // Conversion to/from transformation matrices.
    // Note: JEOD exclusively uses left transformation quaternions. To avoid
    // confusion, this usage is explicitly part of the method names.
    void left_quat_from_transformation(const double T[3][3]);

    void left_quat_to_transformation(double T[3][3]) const;

    // Conversion to/from eigen rotations.
    void left_quat_from_eigen_rotation(double eigen_angle, const double eigen_axis[3]);

    void left_quat_to_eigen_rotation(double * eigen_angle, double eigen_axis[3]) const;

    // Angular displacement between frames in sense of eigen rotation
    void eigen_compare(const Quaternion & compare_to, double * eigen_angle, double eigen_axis[3]) const;

    // Transform a vector.
    void left_quat_transform(const double vec_in[3], double vec_out[3]) const;

    // Compute the quaternion derivative given an angular velocity.
    void compute_left_quat_deriv(const double ang_vel[3], Quaternion & qdot) const;

    // Compute the quaternion derivative given an angular velocity.
    void compute_left_quat_second_deriv(const double ang_vel[3], const double ang_acc[3], Quaternion & qdot) const;

    // Compute the quaternion derivative given an angular velocity,
    // with the quaternion and its derivative expressed as four-vectors.
    static void compute_left_quat_deriv(const double quat[4], const double ang_vel[3], double qdot[4]);

    // Compute the quaternion second derivative given an angular velocity and
    // angular acceleration, with the quaternion and its second derivative
    // expressed as four-vectors.
    static void compute_left_quat_second_deriv(const double quat[4],
                                               const double ang_vel[3],
                                               const double ang_acc[3],
                                               double qddot[4]);

    // SLERP Algorithm - interpolates the shortest and straightest (minimum)
    // geodescic between two quaternions.
    static Quaternion compute_slerp(Quaternion & q1, Quaternion & q2, const double T);
};

} // namespace jeod

#include "quat_inline.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
