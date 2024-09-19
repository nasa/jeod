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
 * @addtogroup Experimental
 * @{
 * @addtogroup ExpMath
 * @{
 *
 * @file
 * Defines the class GaussJordanSolver.
 */

/*
Purpose: ()
Library dependencies: ((../src/gauss_jordan_solver.cc))
*/

#ifndef JEOD_GAUSS_JORDAN_SOLVER_HH
#define JEOD_GAUSS_JORDAN_SOLVER_HH

#include "linear_system_solver.hh"
#include "two_d_array.hh"

#include "utils/container/include/primitive_vector.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Solves a linear system of equations using Gauss-Jordan elimination.
 */
class GaussJordanSolver : public LinearSystemSolver
{
    JEOD_MAKE_SIM_INTERFACES(jeod, GaussJordanSolver)

public:
    /**
     * Vector of doubles.
     */
    using DoubleVectorT = LinearSystemSolver::DoubleVectorT;

    /**
     * Vector of unsigned ints.
     */
    using UnsignedVectorT = JeodPrimitiveVector<unsigned int>::type;

    // Member functions

    /**
     * Default constructor.
     */
    GaussJordanSolver()
    {
        JEOD_REGISTER_CLASS(GaussJordanSolver);
        JEOD_REGISTER_CHECKPOINTABLE(this, avail_rows);
        JEOD_REGISTER_CHECKPOINTABLE(this, pivot_row);
    }

    /**
     * Destructor.
     */
    ~GaussJordanSolver() override
    {
        JEOD_DEREGISTER_CHECKPOINTABLE(this, avail_rows);
        JEOD_DEREGISTER_CHECKPOINTABLE(this, pivot_row);
    }

    GaussJordanSolver(const GaussJordanSolver &) = delete;
    GaussJordanSolver & operator=(const GaussJordanSolver &) = delete;

    /**
     * Set the maximum dimensionality of the problem.
     */
    void set_max_dimensions(unsigned max_dims_in) override
    {
        LinearSystemSolver::set_max_dimensions(max_dims_in);
        augmented_matrix.reserve(max_dims * (max_dims + 1u));
        avail_rows.reserve(max_dims);
        pivot_row.reserve(max_dims);
    }

    /**
     * Set the current dimensionality of the problem.
     */
    void set_n_dimensions(unsigned n_dims) override
    {
        LinearSystemSolver::set_n_dimensions(n_dims);
        augmented_matrix.resize(n_dims, n_dims + 1u);
        avail_rows.resize(n_dims);
        pivot_row.resize(n_dims);
    }

    /**
     * Solve for x in A*x = b.
     */
    unsigned solve(DoubleVectorT & x) override;

protected:
    /**
     * The augmented matrix, initialized to [A | b ]
     */
    TwoDArray augmented_matrix; //!< trick_io(**)

    /**
     * Rows that have not yet been reduced.
     */
    UnsignedVectorT avail_rows; //!< trick_io(**)

    /**
     * Rows that have been reduced.
     */
    UnsignedVectorT pivot_row; //!< trick_io(**)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
