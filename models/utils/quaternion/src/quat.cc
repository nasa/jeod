/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/src/quat.cc
 * Define basic methods for the quaternion class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((quat.cc)
   (quat_from_mat.cc))



*******************************************************************************/
// System includes

// Jeod includes
#include "utils/math/include/numerical.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/quat.hh"
#include "../include/quat_messages.hh"
#include "utils/message/include/message_handler.hh"

#include <cmath>

//! Namespace jeod
namespace jeod
{

/**
 * Construct a quaternion; default constructor.
 */
Quaternion::Quaternion()
{
    if(&scalar + 1 != &vector[0])
    {
        JEOD_THROW(42);
    }
}

/**
 * Construct a pure real quaternion.
 * \param[in] real_part Scalar
 */
Quaternion::Quaternion(const double real_part)
    : scalar(real_part)
{
}

/**
 * Construct a left transformation unit quaternion.
 * \param[in] T Transformation matrix
 */
Quaternion::Quaternion(const double T[3][3])
{
    left_quat_from_transformation(T);
}

/**
 * Compute the minimum interpolation quaternion
 * between a start quarternion and end quaternion
 * \param[in] q1 Starting quaternion
 * \param[in] q2 Ending quaternion
 * \param[in] T Interpolation coefficient between
 * 0.0 and 1.0 representing a rotational scale factor
 * between the intial and final quaternion. When the compute_slerp
 * method is used in a loop to rotate an object from a start and end
 * orientation, a smaller step or change in T results in a smoother
 * object rotation
 */
Quaternion Quaternion::compute_slerp(const Quaternion & q1, const Quaternion & q2, const double T)
{
    // Reject degenerate inputs rather than amplify very small components.
    constexpr double min_norm_sq = 1.0e-15;
    const double q1_norm_sq = q1.norm_sq();
    const double q2_norm_sq = q2.norm_sq();
    if(!std::isfinite(q1_norm_sq) || q1_norm_sq < min_norm_sq)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             QuatMessages::undefined,
                             "q1 not finite or does not have sufficiently large norm for SLERP.");
        return Quaternion();
    }

    if(!std::isfinite(q2_norm_sq) || q2_norm_sq < min_norm_sq)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             QuatMessages::undefined,
                             "q2 not finite or does not have sufficiently large norm for SLERP.");
        return Quaternion();
    }

    if(!std::isfinite(T) || T < 0.0 || T > 1.0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             QuatMessages::undefined,
                             "SLERP interpolation coefficient must be between 0 and 1.");
        return Quaternion();
    }

    // Normalize local copies before interpolation.
    Quaternion q1_copy(q1);
    Quaternion q2_copy(q2);
    q1_copy.normalize();
    q2_copy.normalize();

    // The SLERP Algorithm is as follows
    // qm = ( q1*sin((1-T)*theta) + q2*sin(T*theta) ) / sin(theta)
    // Use shortest-path SLERP, with normalized linear interpolation
    // when the quaternions are nearly identical.
    double cosTheta = (q1_copy.scalar * q2_copy.scalar) + Vector3::dot(q1_copy.vector, q2_copy.vector);
    // Ensure shortest angle is used
    if(cosTheta < 0)
    {
        q2_copy.scale(-1.0);
        cosTheta = -cosTheta;
    }

    double ratio1;
    double ratio2;
    if(cosTheta > 1.0 - 1.0e-12)
    {
        // For nearby quaternions, linear interpolation avoids division by a very small sin(theta).
        ratio1 = 1.0 - T;
        ratio2 = T;
    }
    else
    {
        const double theta = std::acos(cosTheta);
        const double sinTheta = std::sin(theta);
        ratio1 = std::sin((1.0 - T) * theta) / sinTheta;
        ratio2 = std::sin(T * theta) / sinTheta;
    }

    q1_copy.scale(ratio1);
    q2_copy.scale(ratio2);

    Quaternion qm;
    qm.scalar = q1_copy.scalar + q2_copy.scalar;
    Vector3::sum(q1_copy.vector, q2_copy.vector, qm.vector);

    const double qm_norm_sq = qm.norm_sq();
    if(!std::isfinite(qm_norm_sq) || qm_norm_sq < min_norm_sq)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             QuatMessages::undefined,
                             "SLERP produced a nonfinite or insufficiently large quaternion.");
        return Quaternion();
    }

    qm.normalize();

    return qm;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
