/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_point_state.cc
 * Define basic methods for the MassPointState class.
 */

/*******************************************************************************
  Purpose:
    ()

  Reference:
    (((TBS)))

  Assumptions and limitations:
    ((N/A))

  Class:
    (N/A)

  LIBRARY DEPENDENCY:
    ((mass_point_state.cc)
     (utils/quaternion/src/quat.cc)
     (utils/quaternion/src/quat_norm.cc)
     (utils/quaternion/src/quat_to_mat.cc))


*******************************************************************************/

// System includes

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/numerical.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/mass_point_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initialize a mass point.
 */
void MassPointState::initialize_mass_point()
{
    // Set location is the zero vector, transformation to the identity matrix.
    Vector3::initialize(position);
    Matrix3x3::identity(T_parent_this);
    Q_parent_this.make_identity();
}

/*******************************************************************************

  Nomenclature of mathematical formulations.
    x,Q,T     = position, quaternion, transform, respectively.
    A,B,C     = various reference frames.


  Long-hand notations.
    x_A->B:C  = Position of frame B origin with respect to frame A origin,
                expressed in frame A coordinates.
    T_A->B    = Column vector transformation matrix from frame A to frame B.


  Short-hand notations.
    x_A:B     = x_A->B:A (Location of frame B origin in frame A coordinates.)
    T_A:B     = T_A->B
    S_A:B     = {x_A:B, T_A:B}
                In other words, the state of frame A with respect to frame B.


  Operators.
  Conceptually, there are three primitive operators on reference frame states:
  assignment (=), unary negation (-), and addition (+).  Note that addition
  is nonabelian. Subtraction is in terms of addition and negation.

  Negation: S_B:A = - S_A:B
    T_B:A =   T_A:B^T
    x_B:A = - (T_A:B * x_A:B)

  Addition: S_A:C = S_A:B + S_B:C
    T_A:C = T_B:C * T_A:B
    x_A:C = x_A:B + T_A:B^T * x_B:C


  Methods.
    Method       This(input)  Operand  Result
    negate       N/A          S_A:B    this <- -S_A:B = S_B:A
    incr_left    S_B:C        S_A:B    this <- arg + this = S_A:C
    incr_right   S_A:B        S_B:C    this <- this + arg = S_A:C
    decr_left    S_A:C        S_A:B    this <- (-arg) + this = S_B:C
    decr_right   S_A:C        S_B:C    this <- this + (-arg) = S_A:B


  Shortcut:
    There is no need to transform a vector if the transformation is an identity.
    The scalar part of the left transformation quaternion is exactly one in the
    case of an identity transformation.

*******************************************************************************/

/**
 * Copy a mass point state, negated.
 * \param[in] source Source state
 */

/*******************************************************************************
  Function: negate
  Purpose: (Copy a mass point state, negated.)
  Class: (N/A)
*******************************************************************************/
void MassPointState::negate(const MassPointState & source)
{
    // Relevant equations:
    //   T_B:A =   T_A:B^T
    //   x_B:A = - (T_A:B * x_A:B)

    // Transform the matrix and translational state to the alternate frame.
    // This only needs to be performed for nontrivial transformation matrices.
    if(!Numerical::compare_exact(source.Q_parent_this.scalar, 1.0))
    {
        source.Q_parent_this.conjugate(Q_parent_this);
        Matrix3x3::transpose(source.T_parent_this, T_parent_this);
        Vector3::transform_transpose(T_parent_this, source.position, position);
    }

    // Identity transform: Just copy position.
    else
    {
        Matrix3x3::identity(T_parent_this);
        Q_parent_this.make_identity();
        Vector3::copy(source.position, position);
    }

    // The above made the position have the wrong sign. Fix that.
    Vector3::negate(position);
}

/**
 * Compute S_A:C = S_A:B + S_B:C,
 * with this initially containing S_B:C,
 * the supplied argument containing S_A:B, and
 * the resultant composition of states stored in this.
 * \param[in] s_ab Left addend
 */
void MassPointState::incr_left(const MassPointState & s_ab)
{
    // Relevant equations:
    //   T_A:C = T_B:C * T_A:B
    //   x_A:C = x_A:B + T_A:B^T * x_B:C

    // Order dependency:
    //   Quantity   Uses S_B:C items  Uses S_A:C items   Order
    //   T_A:C      T                 None               1
    //   x_A:C      x                 None               2

    // Compute T_A:C and x_A:C via
    //   T_A:C = T_B:C * T_A:B
    //   x_A:C = x_A:B + T_A:B^T * x_B:C
    // Shortcut: only do the transformations if T_A:B is not the identity matrix.
    if(!Numerical::compare_exact(s_ab.Q_parent_this.scalar, 1.0))
    {
        // Compute Q_A:C = Q_B:C * Q_A:B (and then normalize result)
        Q_parent_this.multiply(s_ab.Q_parent_this);
        Q_parent_this.normalize();

        // Compute the corresponding transformation matrix.
        compute_transformation();

        // Transform x_B:C to frame A.
        Vector3::transform_transpose(s_ab.T_parent_this, position);

        // No else. x_B:C is already in frame in as T_A:B is identity.
    }

    // Add X_A:B to the state.
    Vector3::incr(s_ab.position, position);
}

/**
 * Compute S_A:C = S_A:B + S_B:C,
 * with this initially containing S_A:B,
 * the supplied argument containing S_B:C, and
 * the resultant composition of states stored in this.
 * \param[in] s_bc Right addend
 */
void MassPointState::incr_right(const MassPointState & s_bc)
{
    // Relevant equations:
    //   T_A:C = T_B:C * T_A:B
    //   x_A:C = x_A:B + T_A:B^T * x_B:C

    // Order dependency:
    //   Quantity   Uses S_A:B items  Uses S_A:C items   Order
    //   T_A:C      T                 None               2
    //   x_A:C      x, T              None               1

    // Compute x_A:C, v_A:C, and T_A:C via
    //   T_A:C = T_B:C * T_A:B
    //   x_A:C = x_A:B + T_A:B^T * x_B:C
    // Shortcut: only do the transformations if T_A:B is not the identity matrix.
    if(!Numerical::compare_exact(Q_parent_this.scalar, 1.0))
    {
        // Compute x_A:C.
        // Note: This must be computed before computing T_A:C as it uses T_A:B.
        Vector3::transform_transpose_incr(T_parent_this, s_bc.position, position);

        // Compute Q_A:C = Q_B:C * Q_A:B (and then normalize result)
        Q_parent_this.multiply_left(s_bc.Q_parent_this);
        Q_parent_this.normalize();

        // Compute the corresponding transformation matrix.
        compute_transformation();
    }

    // Shortcuts for the case T_A:B is identity.
    else
    {
        Q_parent_this = s_bc.Q_parent_this;
        Matrix3x3::copy(s_bc.T_parent_this, T_parent_this);
        Vector3::incr(s_bc.position, position);
    }
}

/**
 * Compute S_B:C = (-S_A:B) + S_A:C,
 * with this initially containing S_A:C,
 * the supplied argument containing S_A:B, and
 * the resultant composition of states stored in this.
 * \param[in] s_ab Left addend
 */
void MassPointState::decr_left(const MassPointState & s_ab)
{
    // Relevant equations:
    //   T_B:C = T_A:C * T_A:B^T
    //   x_B:C = T_A:B * (x_A:C - x_A:B)

    // Order dependency:
    //   Quantity   Uses S_A:C items  Uses S_B:C items   Order
    //   T_B:C      T                 None               1
    //   x_B:C      x                 None               2

    // Compute T_B:C and x_B:C via
    //   T_B:C = T_A:C * T_A:B^T
    //   x_B:C = T_A:B * (x_A:C - x_A:B)
    // Shortcut: only do the transformations if T_A:B is not the identity matrix.
    if(!Numerical::compare_exact(s_ab.Q_parent_this.scalar, 1.0))
    {
        // Compute Q_B:C = Q_A:C * Q_A:B^Q
        Q_parent_this.multiply_conjugate(s_ab.Q_parent_this);
        Q_parent_this.normalize();

        // Compute the corresponding transformation matrix.
        compute_transformation();

        // Compute x_B:C.
        Vector3::decr(s_ab.position, position);
        Vector3::transform(s_ab.T_parent_this, position);
    }

    // Shortcuts for the case T_A:B is identity.
    else
    {
        Vector3::decr(s_ab.position, position);
    }
}

/**
 * Compute S_A:B = S_A:C + (-S_B:C)
 * with this initially containing S_A:C,
 * the supplied argument containing S_B:C, and
 * the resultant composition of states stored in this.
 * \param[in] s_bc Left addend
 */
void MassPointState::decr_right(const MassPointState & s_bc)
{
    // Relevant equations:
    //   T_A:B = T_B:C^T * T_A:C
    //   x_A:B = x_A:C - T_A:B^T * x_B:C

    // Order dependency:
    //   Quantity   Uses S_A:C items  Uses S_A:B items   Order
    //   T_A:B      T                 None               1
    //   x_A:B      x                 T                  2

    // Compute T_A:B via
    //   T_A:B = T_B:C^T * T_A:C
    // Shortcut: only do the transformations if T_B:C is not the identity matrix.
    if(!Numerical::compare_exact(s_bc.Q_parent_this.scalar, 1.0))
    {
        Q_parent_this.multiply_left_conjugate(s_bc.Q_parent_this);
        Q_parent_this.normalize();
        compute_transformation();

        // No else. T_A:B = T_A:C as T_B:C is identity.
    }

    // Compute x_A:B via
    //   x_A:B = x_A:C - T_A:B^T * x_B:C
    if(!Numerical::compare_exact(Q_parent_this.scalar, 1.0))
    {
        Vector3::transform_transpose_decr(T_parent_this, s_bc.position, position);
    }
    // Shortcuts for the case T_A:B is identity.
    else
    {
        Vector3::decr(s_bc.position, position);
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
