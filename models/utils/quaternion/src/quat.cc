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
#include <fstream> // delete this later
#include <iomanip> // delete this later

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
Quaternion Quaternion::compute_slerp(Quaternion & q1, Quaternion & q2, const double T)
{
    // SLERP is only valid for unit quaternions
    q1.normalize();
    q2.normalize();

    // The SLERP Algorithm is as follows
    // qm = ( q1*sin((1-T)*theta) + q2*sin(T*theta) ) / sin(theta)

    Quaternion qm;

    double cosTheta = (q1.scalar * q2.scalar) + Vector3::dot(q1.vector, q2.vector);
    // Ensure shortest angle is used
    if(cosTheta < 0)
    {
        // q2 = -q2;
        q2.scalar = -q2.scalar;
        q2.vector[0] = -q2.vector[0];
        q2.vector[1] = -q2.vector[1];
        cosTheta = -cosTheta;
    }

    // If q1 = q2 or q1 = -q2 then theta = 0
    // Therefore, qm = q1
    if(std::abs(cosTheta) > 1.0 || Numerical::compare_exact(std::abs(cosTheta), 1.0))
    {
        return q1;
    }

    // Intermediate calcs. Angles are in radians
    double theta = std::acos(cosTheta);
    double sinTheta = std::sin(theta);
    // If theta = 180 deg, result is undefinted
    if(std::abs(sinTheta) < 0.0000000000001)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             QuatMessages::undefined,
                             "The quaternions provided result in an undefined quaternion interpolation.");
    }
    double ratio1 = std::sin((1 - T) * theta) / sinTheta;
    double ratio2 = std::sin(T * theta) / sinTheta;

    qm.scalar = ((q1.scalar * ratio1) + (q2.scalar * ratio2));
    for(int i = 0; i < 3; i++)
    {
        qm.vector[i] = (q1.vector[i] * ratio1 + q2.vector[i] * ratio2);
    }

    // Normalize the resulting interpolation quaternion
    qm.normalize();

    return qm;
    // all done!
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
