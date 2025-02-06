/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Orientation
 * @{
 *
 * @file models/utils/orientation/src/orientation.cc
 * Define methods for the NewOrientation class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((orientation.cc)
   (eigen_rotation.cc)
   (euler_angles.cc)
   (orientation_messages.cc)
   (utils/message/src/message_handler.cc)
   (utils/quaternion/src/quat.cc)
   (utils/quaternion/src/quat_from_mat.cc)
   (utils/quaternion/src/quat_norm.cc)
   (utils/quaternion/src/quat_to_eigenrot.cc)
   (utils/quaternion/src/quat_to_mat.cc))



*******************************************************************************/

// System includes
#include <cmath>

// Jeod includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/orientation.hh"
#include "../include/orientation_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Construct an Orientation instance from a transformation matrix.
 * \param[in] trans_in Transformation matrix
 */
Orientation::Orientation(const double trans_in[3][3])
    : data_source(InputMatrix),
      have_transformation_(true)
{
    Matrix3x3::copy(trans_in, trans);
}

/**
 * Construct an Orientation instance from a Quaternion.
 * \param[in] quat_in Quaternion
 */
Orientation::Orientation(const Quaternion & quat_in)
    : data_source(InputQuaternion),
      quat(quat_in),
      have_quaternion_(true)
{
}

/**
 * Construct an Orientation instance from an eigen rotation.
 * \param[in] eigen_angle_in Rotation angle\n Units: r
 * \param[in] eigen_axis_in Rotation axis, unit vector
 */
Orientation::Orientation(double eigen_angle_in, const double eigen_axis_in[3])
    : data_source(InputEigenRotation),
      eigen_angle(eigen_angle_in),
      have_eigen_rotation_(true)
{
    Vector3::copy(eigen_axis_in, eigen_axis);
}

/**
 * Construct an Orientation instance from an Euler rotation.
 * \param[in] sequence_in Euler sequence
 * \param[in] angles_in Euler angles\n Units: r
 */
Orientation::Orientation(EulerSequence sequence_in, const double angles_in[3])
    : data_source(InputEulerRotation),
      euler_sequence(sequence_in),
      have_euler_angles_(true)
{
    Vector3::copy(angles_in, euler_angles);
}

/*******************************************************************************
 * Support methods
 ******************************************************************************/

/**
 * Forget that we have any data.
 * Note that this method does not reset the euler_sequence member;
 * that is intentional.
 */

/*
Purpose:
  (Forget that we have any data.
   Note that this method does not reset the euler_sequence member;
   that is intentional.)
*/
void Orientation::reset()
{
    data_source = InputNone;
    have_quaternion_ = have_transformation_ = have_eigen_rotation_ = have_euler_angles_ = false;
}

/**
 * Mark the item specified by the data_source as available.
 * Note that this method doesn't compute any products.
 *
 * \par Assumptions and Limitations
 *  - The data_source member datum is valid.
 */
void Orientation::mark_input_as_available()
{
    // Ensure the availability flag for the user input is set.
    switch(data_source)
    {
        case InputMatrix:
            have_transformation_ = true;
            break;

        case InputQuaternion:
            have_quaternion_ = true;
            break;

        case InputEigenRotation:
            have_eigen_rotation_ = true;
            break;

        case InputEulerRotation:
            have_euler_angles_ = true;
            break;

        // Enforce the assumption of a valid data_source.
        case InputNone:
        default:
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  OrientationMessages::invalid_enum,
                                  "Invalid data_source value=%d",
                                  static_cast<int>(data_source));
            return;
    }
}

/*******************************************************************************
 * compute_<single_representation> and compute_<multiple_representation> methods
 *
 * Each compute_<single_representation> method computes the single
 * representation of the orientation if possible to do so. (The only way this is
 * not possible is when the orientation object is void of data.)
 *  - No computation is needed if the requested item is already available.
 *  - A direct conversion from the data source is used if possible.
 *  - An indirect path is used in those cases where a direct conversion is not
 *    available.
 *
 * Direct conversions exist for the follow representation schemes:
 * To           From         Method
 * Matrix       Quaternion   Quaternion::left_quat_to_transformation
 *              Eigen rot    Orientation::compute_matrix_from_eigen_rotation
 *              Euler seq    Orientation::compute_matrix_from_euler_angles
 * Quaternion   Matrix       Quaternion::left_quat_from_transformation
 *              Eigen rot    Quaternion::left_quat_from_eigen_rotation
 * Eigen rot    Quaternion   Quaternion::left_quat_to_eigen_rotation
 *              Matrix       Orientation::compute_eigen_rotation_from_matrix
 * Euler seq    Matrix       Orientation::compute_euler_angles_from_matrix
 *
 * Missing from the above are methods to transform a quaternion or a single
 * axis rotation to an Euler rotation sequence and methods to transform an
 * Euler rotation sequence to a quaternion or a single axis rotation.
 *
 * The compute_<multiple_representation> methods are convenience methods that
 * compute more than one representation of the orientation from the source.
 ******************************************************************************/

/**
 * Compute the transformation matrix from the source.
 */

/*
Purpose:
  (Compute the transformation matrix from the source.)
*/
void Orientation::compute_transform()
{
    // Matrix is already available: We're done.
    if(have_transformation_)
    {
        return;
    }

    // Ensure the availability flag for the user input is set.
    mark_input_as_available();

    // A direct path exists from each data source to a transformation matrix.
    // Use the data source as the definitive source.
    switch(data_source)
    {
        // Source is a matrix: Nothing to do.
        case InputMatrix:
            break;

        // Source is a quaternion: Extract the matrix from the quaternion.
        case InputQuaternion:
            quat.left_quat_to_transformation(trans);
            break;

        // Source is a single axis rotation: Extract the matrix from that.
        case InputEigenRotation:
            compute_matrix_from_eigen_rotation();
            break;

        // Source is an Euler rotation: Extract the matrix from that.
        case InputEulerRotation:
            compute_matrix_from_euler_angles();
            break;

        // Invalid marker: Just return.
        // A complaint was just issued by the call to mark_input_as_available.
        case InputNone:
        default:
            return;
    }

    have_transformation_ = true;
}

/**
 * Compute the left transformation quaternion from the source.
 */
void Orientation::compute_quaternion()
{
    // Quaternion is already available: We're done.
    if(have_quaternion_)
    {
        return;
    }

    // Ensure the availability flag for the user input is set.
    mark_input_as_available();

    // Compute the quaternion based on the source, using a direct path
    // if possible.
    switch(data_source)
    {
        // Source is a quaternion: Nothing to do.
        case InputQuaternion:
            break;

        // Source is a matrix: Extract the quaternion from the matrix.
        case InputMatrix:
            quat.left_quat_from_transformation(trans);
            break;

        // Source is a single axis rotation: Extract the quaternion from that.
        case InputEigenRotation:
            quat.left_quat_from_eigen_rotation(eigen_angle, eigen_axis);
            break;

        // Source is an Euler rotation: Need to go through the matrix.
        case InputEulerRotation:
            compute_quaternion_from_euler_angles();
            break;

        // Invalid marker: Just return.
        // A complaint was just issued by the call to mark_input_as_available.
        case InputNone:
        default:
            return;
    }

    have_quaternion_ = true;
}

/**
 * Compute the eigen rotation from the source.
 */
void Orientation::compute_eigen_rotation()
{
    // Eigen rotation is already available: We're done.
    if(have_eigen_rotation_)
    {
        return;
    }

    // Ensure the availability flag for the user input is set.
    mark_input_as_available();

    // Compute the eigen rotation based on the source, using a direct path
    // if possible.
    switch(data_source)
    {
        // Source is an eigen rotation: Nothing to do.
        case InputEigenRotation:
            break;

        // Source is a matrix: Extract the eigen rotation from the matrix.
        case InputMatrix:
            compute_eigen_rotation_from_matrix();
            break;

        // Source is a quaternion: Extract the eigen rotation from the matrix.
        case InputQuaternion:
            quat.left_quat_to_eigen_rotation(&eigen_angle, eigen_axis);
            break;

        // Source is an Euler rotation: Need to go through the matrix.
        case InputEulerRotation:
            if(!have_transformation_)
            {
                have_transformation_ = true;
                compute_matrix_from_euler_angles();
            }
            compute_eigen_rotation_from_matrix();
            break;

        // Invalid marker: Just return.
        // A complaint was just issued by the call to mark_input_as_available.
        case InputNone:
        default:
            return;
    }

    have_eigen_rotation_ = true;
}

/**
 * Compute the eigen rotation from the source.
 */
void Orientation::compute_euler_angles()
{
    // Euler angles are already available: We're done.
    if(have_euler_angles_)
    {
        return;
    }

    // Validate the value of the euler_sequence member.
    // Note that an invalid value means the object is left unchanged.
    if((euler_sequence < EulerXYZ) || (euler_sequence > EulerZYZ))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              OrientationMessages::invalid_enum,
                              "The euler_sequence data member has not been set or is invalid; "
                              "value=%d",
                              static_cast<int>(euler_sequence));
        return;
    }

    // Ensure the availability flag for the user input is set.
    mark_input_as_available();

    // Compute the Euler angles based on the source, using a direct path
    // if possible.
    switch(data_source)
    {
        // Source is an Euler rotation: Nothing to do.
        case InputEulerRotation:
            break;

        // Source is a matrix: Extract the Euler rotation from the matrix.
        case InputMatrix:
            compute_euler_angles_from_matrix();
            break;

        // Source is a quaternion: Need to go through the matrix.
        case InputQuaternion:
            if(!have_transformation_)
            {
                have_transformation_ = true;
                quat.left_quat_to_transformation(trans);
            }
            compute_euler_angles_from_matrix();
            break;

        // Source is a single axis rotation: Need to go through the matrix.
        case InputEigenRotation:
            if(!have_transformation_)
            {
                have_transformation_ = true;
                compute_matrix_from_eigen_rotation();
            }
            compute_euler_angles_from_matrix();
            break;

        // Invalid marker: Just return.
        // A complaint was just issued by the call to mark_input_as_available.
        case InputNone:
        default:
            return;
    }

    have_euler_angles_ = true;
}

/**
 * Compute all represented charts on SO3 from the specified source.
 */
void Orientation::compute_all_products()
{
    compute_transform();
    compute_quaternion();
    compute_eigen_rotation();
    compute_euler_angles();
}

/**
 * Compute the transformation matrix and quaternion.
 */
void Orientation::compute_transformation_and_quaternion()
{
    compute_transform();
    compute_quaternion();
}

/*******************************************************************************
 * Getters
 * Except for get_euler_sequence(), each of these gets one specific item, using
 * the appropriate compute_xxx method to ensure the requested item exists.
 *
 * get_euler_sequence() returns the value of the euler_sequence data member.
 ******************************************************************************/

/**
 * Accessor for the transformation matrix.
 * \param[out] trans_out Copy of the transformation matrix
 */

/*
Purpose:
  (Accessor for the transformation matrix.)
*/
void Orientation::get_transform(double trans_out[3][3])
{
    // Compute the matrix if possible.
    compute_transform();

    // The above worked: copy the matrix to the provided output.
    if(have_transformation_)
    {
        Matrix3x3::copy(trans, trans_out);
    }
    // No else: The only thing that can be done at this point is complain, and
    // compute_quaternion() will have already done just that.
}

/**
 * Accessor for the left transformation quaternion.
 * \param[out] quat_out Copy of the quaternion
 */
void Orientation::get_quaternion(Quaternion & quat_out)
{
    // Compute the quaternion if possible.
    compute_quaternion();

    // The above worked: copy the quaternion to the provided output.
    if(have_quaternion_)
    {
        quat_out = quat;
    }
    // No else: The only thing that can be done at this point is complain, and
    // compute_quaternion() will have already done just that.
}

/**
 * Accessor for the eigen rotation.
 * \param[out] eigen_angle_out Copy of the single axis rotation angle\n Units: r
 * \param[out] eigen_axis_out Copy of the single axis rotation axis
 */
void Orientation::get_eigen_rotation(double * eigen_angle_out, double eigen_axis_out[3])
{
    // Compute the eigen rotation if possible.
    compute_eigen_rotation();

    // The above worked: copy the eigen rotation to the provided outputs.
    if(have_eigen_rotation_)
    {
        *eigen_angle_out = eigen_angle;
        Vector3::copy(eigen_axis, eigen_axis_out);
    }
    // No else: The only thing that can be done at this point is complain, and
    // compute_eigen_rotation() will have already done just that.
}

/**
 * Accessor for the Euler angles.
 * \param[out] sequence Copy of the Euler sequence
 * \param[out] angles Copy of the Euler angles\n Units: r
 */
void Orientation::get_euler_angles(EulerSequence * sequence, double angles[3])
{
    // Compute the Euler rotation if possible.
    compute_euler_angles();

    // The above worked: copy the Euler angles to the provided outputs.
    if(have_euler_angles_)
    {
        *sequence = euler_sequence;
        Vector3::copy(euler_angles, angles);
    }
    // No else: The only thing that can be done at this point is complain, and
    // compute_eigen_rotation() will have already done just that.
}

/**
 * Accessor for the Euler angles.
 * \param[out] angles Copy of the Euler angles\n Units: r
 */
void Orientation::get_euler_angles(double angles[3])
{
    // Compute the Euler rotation if possible.
    compute_euler_angles();

    // The above worked: copy the Euler angles to the provided outputs.
    if(have_euler_angles_)
    {
        Vector3::copy(euler_angles, angles);
    }
    // No else: The only thing that can be done at this point is complain, and
    // compute_eigen_rotation() will have already done just that.
}

/**
 * Accessor for the Euler sequence data member.
 * @return Euler sequence data member
 */
Orientation::EulerSequence Orientation::get_euler_sequence()
{
    return euler_sequence;
}

/*******************************************************************************
 * Setters
 * Except for set_euler_sequence(), each of these sets one specific item.
 * set_euler_sequence() is again a bit of an odd ball.
 ******************************************************************************/

/**
 * Reset the instance with a new matrix.
 * \param[in] trans_in New transformation matrix
 */

/*
Purpose:
  (Reset the instance with a new matrix.)
*/
void Orientation::set_transform(const double trans_in[3][3])
{
    reset();

    have_transformation_ = true;
    data_source = InputMatrix;
    Matrix3x3::copy(trans_in, trans);
}

/**
 * Reset the instance with a new quaternion.
 * \param[in] quat_in New quaternion
 */
void Orientation::set_quaternion(const Quaternion & quat_in)
{
    reset();

    have_quaternion_ = true;
    data_source = InputQuaternion;
    quat = quat_in;
}

/**
 * Reset the instance with a new eigen rotation.
 * \param[in] eigen_angle_in New single axis rotation angle
 * \param[in] eigen_axis_in New single axis rotation axis
 */
void Orientation::set_eigen_rotation(double eigen_angle_in, const double eigen_axis_in[3])
{
    reset();

    have_eigen_rotation_ = true;
    data_source = InputEigenRotation;
    eigen_angle = eigen_angle_in;
    Vector3::copy(eigen_axis_in, eigen_axis);
}

/**
 * Reset the instance with a new Euler rotation.
 * \param[in] sequence New Euler sequence
 * \param[in] angles New Euler angles\n Units: r
 */
void Orientation::set_euler_angles(EulerSequence sequence, const double angles[3])
{
    // Validate the provided enum value.
    if((sequence < EulerXYZ) || (sequence > EulerZYZ))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              OrientationMessages::invalid_enum,
                              "Invalid sequence argument, value=%d",
                              static_cast<int>(sequence));
        return;
    }

    // OK, its valid. The rest of the method follows the same pattern
    // as the other setters.

    reset();

    have_euler_angles_ = true;
    data_source = InputEulerRotation;
    euler_sequence = sequence;
    Vector3::copy(angles, euler_angles);
}

/**
 * Reset the instance with a new Euler rotation.
 *
 * \par Assumptions and Limitations
 *  - The euler_sequence data member must have previously been set to
 *     a valid value.
 * \param[in] angles New Euler angles\n Units: r
 */
void Orientation::set_euler_angles(const double angles[3])
{
    // Validate the value of the euler_sequence member.
    // Note that an invalid value means the object is left unchanged.
    if((euler_sequence < EulerXYZ) || (euler_sequence > EulerZYZ))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              OrientationMessages::invalid_enum,
                              "The euler_sequence data member has not been set or is invalid; "
                              "value=%d",
                              static_cast<int>(euler_sequence));
        return;
    }

    // OK, its valid. The rest of the method follows the same pattern
    // as the other setters.

    reset();

    have_euler_angles_ = true;
    data_source = InputEulerRotation;
    Vector3::copy(angles, euler_angles);
}

/**
 * Set the euler_sequence data member.
 * \param[in] sequence New Euler sequence
 */
void Orientation::set_euler_sequence(EulerSequence sequence)
{
    // Validate the provided enum value.
    if((sequence < EulerXYZ) || (sequence > EulerZYZ))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              OrientationMessages::invalid_enum,
                              "Invalid sequence argument, value=%d",
                              static_cast<int>(sequence));
        return;
    }

    // Set the euler_sequence from the input value.
    if(euler_sequence != sequence)
    {
        // Clear knowledge of the current sequence, but preserve current inputs.
        clear_euler_sequence();

        // Set the sequence as requested.
        have_euler_angles_ = false;
        euler_sequence = sequence;
    }
    // No else: There's nothing to do if there is no change.
}

/**
 * Reset the euler_sequence data member.
 * Issues arise if the data source is the Euler rotation sequence.
 * The resolution is to preserve the existing input elsewhere.
 */
void Orientation::clear_euler_sequence()
{
    // Transfer the data source to the matrix if the data source is the
    // existing Euler rotation.
    if(data_source == InputEulerRotation)
    {
        if(!have_transformation_)
        {
            have_transformation_ = true;
            compute_matrix_from_euler_angles();
        }
        if(!have_quaternion_)
        {
            have_quaternion_ = true;
            compute_quaternion_from_euler_angles();
        }
        data_source = InputMatrix;
    }

    have_euler_angles_ = false;
    euler_sequence = NoSequence;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
