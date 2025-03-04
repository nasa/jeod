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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup ExpMath
 * @{
 *
 * @file
 * Defines various type definitions used in the constraints concept.
 */

/*
Purpose: ()
ICG: (No)
SWIG: (No)
*/

#ifndef JEOD_SOLVER_TYPES_HH
#define JEOD_SOLVER_TYPES_HH

#include <type_traits>
#include <utility>

//! Namespace jeod
namespace jeod
{

/**
 * Define various types used in the constraints concept.
 * This would be better as a namespace, but some versions of Trick
 * and SWIG have problems with namespaces.
 */
class SolverTypes
{
public:
    // Data types

    /**
     * Vector3T foo is equivalent to double foo[3].
     */
    using Vector3T = double[3];

    /**
     * ConstVector3T foo is equivalent to const double foo[3].
     */
    using ConstVector3T = const double[3];

    /**
     * A pointer to a Vector3T.
     */
    using Vector3PointerT = Vector3T *;

    /**
     * A pointer to a ConstVector3T.
     */
    using ConstVector3PointerT = ConstVector3T *;

    /**
     * An L-value reference to a Vector3T.
     */
    using Vector3RefT = Vector3T &;

    /**
     * An L-value reference to a ConstVector3T.
     */
    using ConstVector3RefT = ConstVector3T &;

    /**
     * aka double*
     */
    using DecayedVector3T = double *;
    /**
     * aka const double*
     */
    using ConstDecayedVector3T = const double *;

    /**
     * Matrix3x3T foo is equivalent to double foo[3][3]
     */
    using Matrix3x3T = double[3][3];

    /**
     * ConstMatrix3x3T foo is equivalent to const double foo[3][3]
     */
    using ConstMatrix3x3T = const double[3][3];

    /**
     * A pointer to a Matrix3x3T.
     */
    using Matrix3x3PointerT = Matrix3x3T *;

    /**
     * A pointer to a ConstMatrix3x3T.
     */
    using ConstMatrix3x3PointerT = ConstMatrix3x3T *;

    /**
     * An L-value reference to a Matrix3x3T.
     */
    using Matrix3x3RefT = Matrix3x3T &;

    /**
     * An L-value reference to a ConstMatrix3x3T.
     */
    using ConstMatrix3x3RefT = ConstMatrix3x3T &;
    using DecayedMatrix3x3T = Vector3T *;
    using ConstDecayedMatrix3x3T = ConstVector3T *;

    using IndexPairT = std::pair<unsigned, unsigned>;

    // This is a class that is not a class.
    SolverTypes() = delete;
    ~SolverTypes() = default;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
