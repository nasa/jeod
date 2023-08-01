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
 * Defines the class LinearSystemSolver.
 */

/*
Purpose: ()
*/


#ifndef JEOD_LINEAR_SYSTEM_SOLVER_HH
#define JEOD_LINEAR_SYSTEM_SOLVER_HH


#include "solver_types.hh"
#include "sub_vector_view.hh"
#include "two_d_array.hh"

#include "utils/container/include/primitive_vector.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * Represents a linear system of equations, A*x = b, where A is conceptually
 * an NxN matrix, and x and b are conceptually N-vectors. This class
 * solves for x in A*x = b. How it does that is up to a derived class.
 * `solve` is pure virtual in this base class template.
 *
 * An implementor of an instantiable derived class of this class template
 * must implement the `solve` method.
 *
 * A user of this class template
 *  - Sets the number of degrees of freedom N,
 *  - Populates the A matrix and b vector, and
 *  - Requests the object to solve for x.
 *
 * Users must not assume that `solve` leaves the populated A matrix and b vector
 * untouched. To the contrary, an instantiable solver might well modify these
 * elements in the process of solving the matrix equation. It is up to the user
 * to fully populate the A matrix and b vector prior to calling `solve`.
 */
class LinearSystemSolver
{
    JEOD_MAKE_SIM_INTERFACES(LinearSystemSolver)

public:

    // Member types.

    /**
     * Vector of doubles.
     */
    typedef JeodPrimitiveVector<double>::type DoubleVectorT;

#ifndef SWIG
    /**
     * A slice of the A matrix.
     */
    typedef SubArrayView<TwoDArray, double> AMatrixViewT;

    /**
     * A slice of the b vector.
     */
    typedef SubVectorView<std::vector<double>, double> BVectorViewT;
#endif


    // Member functions

    /**
     * Default constructor.
     */
    LinearSystemSolver ()
    {
        JEOD_REGISTER_NONEXPORTED_CLASS(LinearSystemSolver);
        // FIXME: Make TwoDArray checkpointable.
        // FIXME: Uncomment the following after fixing the above.
        // JEOD_REGISTER_CHECKPOINTABLE (this, a_matrix);
        JEOD_REGISTER_CHECKPOINTABLE (this, b_vector);
    }

    /**
     * Destructor.
     */
    virtual ~LinearSystemSolver ()
    {
        // FIXME: Uncomment the following after making TwoDArray checkpointable.
        // JEOD_DEREGISTER_CHECKPOINTABLE (this, a_matrix);
        JEOD_DEREGISTER_CHECKPOINTABLE (this, b_vector);
    }


    /**
     * Set the maximum dimensionality of the problem.
     */
    virtual void set_max_dimensions (unsigned max_dims_in)
    {
        if (max_dims < max_dims_in)
        {
            max_dims = max_dims_in;
            a_matrix.reserve(max_dims*max_dims);
            b_vector.reserve(max_dims);
        }
    }

    /**
     * Set the current dimensionality of the problem.
     */
    virtual void set_n_dimensions (unsigned n_dims)
    {
        set_max_dimensions (n_dims);
        a_matrix.resize(n_dims, n_dims);
        b_vector.resize(n_dims);
        n_dimensions = n_rows = n_cols = n_dims;
    }

#ifndef SWIG
    /**
     * Create a view to a portion of the A matrix.
     */
    AMatrixViewT make_a_matrix_view(
        const SolverTypes::IndexPairT& row_range,
        const SolverTypes::IndexPairT& col_range)
    {
        return AMatrixViewT(a_matrix, row_range, col_range);
    }

    /**
     * Create a view to a portion of the b vector.
     */
    BVectorViewT make_b_vector_view(
        const SolverTypes::IndexPairT& row_range)
    {
        return BVectorViewT(b_vector, row_range);
    }
#endif


    /**
     * Solve for x in A*x = b.
     */
    virtual unsigned solve(DoubleVectorT& x) = 0;


protected:

    /**
     * The matrix A in A*x = b.
     */
    TwoDArray a_matrix; //!< trick_io(**)

    /**
     * The vector b in A*x = b.
     */
    DoubleVectorT b_vector; //!< trick_io(**)

    /**
     * The maximum number of dimensions, to scope sizing of A and b.
     */
    unsigned max_dims; //!< trick_units(--)

    /**
     * The current dimensionality of the problem.
     */
    unsigned n_dimensions; //!< trick_units(--)

    /**
     * The number of rows currently in use.
     * By default, this is the current dimensionality of the problem.
     */
    unsigned n_rows; //!< trick_units(--)

    /**
     * The number of columns currently in use.
     * By default, this is the current dimensionality of the problem.
     */
    unsigned n_cols; //!< trick_units(--)

private:
    // The copy constructor and copy assignment operator are not implemented
    // to avoid erroneous copies.
    LinearSystemSolver (const LinearSystemSolver&);
    LinearSystemSolver& operator= (const LinearSystemSolver&);

};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
