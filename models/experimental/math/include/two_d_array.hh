//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * Defines the class TwoDArray.
 */

/*
Purpose: ()
*/


#ifndef JEOD_TWO_D_ARRAY_HH
#define JEOD_TWO_D_ARRAY_HH


#include "column_view.hh"
#include "row_view.hh"
#include "solver_types.hh"
#include "sub_array_view.hh"

#include "utils/container/include/primitive_vector.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/sim_interface/include/jeod_class.hh"

#include <type_traits>



//! Namespace jeod 
namespace jeod {

/**
 * Represents a two dimensional array, implemented as a vector that is
 * indexed as a row major two dimensional array.
 * The function call operator provides unprotected access to an element
 * of the array. The member function at() ensures that the indexed element
 * is valid.
 */
class TwoDArray
{
    JEOD_MAKE_SIM_INTERFACES(TwoDArray)

public:

    // Member types.

    /**
     * Vector of doubles.
     * The data contents are stored in a DoubleVectorT, with 2D values stored
     * in this 1D array using either row-major or column-major indexing.
     */
    typedef JeodPrimitiveVector<double>::type DoubleVectorT;


    // Member functions.

    /**
     * Default constructor.
     */
    TwoDArray ()
    :
        repr(),
        max_elems(0),
        n_rows(0),
        n_cols(0)
    {
        register_contents();
    }

    /**
     * Destructor.
     */
    virtual ~TwoDArray ()
    {
        deregister_contents();
    }


    // The copy constructor, move constructor, copy assignment operator, and
    // move assignment operators work just fine.
    TwoDArray (const TwoDArray&) = default;
    TwoDArray& operator= (const TwoDArray&) = default;
#ifndef SWIG
    TwoDArray (TwoDArray&&) = default;
    TwoDArray& operator= (TwoDArray&&) = default;
#endif


#ifndef SWIG
    /**
     * Size function.
     * @return The number of rows and columns in the view, as a pair.
     */
    virtual SolverTypes::IndexPairT size() const
    {
        return SolverTypes::IndexPairT(n_rows, n_cols);
    }
#endif


    /**
     * Reserve storage for at least max_elems_in elements.
     * @param max_elems_in  Amount of storage to be reserved.
     */
    void reserve (unsigned max_elems_in)
    {
        if (max_elems < max_elems_in)
        {
            max_elems = max_elems_in;
            repr.reserve(max_elems);
        }
    }

    /**
     * Resize the array to nrows_in by n_cols_in.
     * @param n_rows_in  The number of rows in the array.
     * @param n_cols_in  The number of columns in the array.
     */
    void resize (unsigned n_rows_in, unsigned n_cols_in)
    {
        unsigned n_elems = n_rows_in*n_cols_in;
        n_rows = n_rows_in;
        n_cols = n_cols_in;
        reserve (n_elems);
        if (repr.size() < n_elems)
        {
            repr.resize(n_elems);
        }
    }


    /**
     * Non-const row major element access to the underlying representation.
     * Warning: This is unprotected.
     * @param i_row  The row number, zero based.
     * @param i_col  The column number, zero based.
     * @return Reference to element at i_row, i_col.
     */
    double& operator() (unsigned i_row, unsigned i_col)
    {
        return repr[i_row*n_cols + i_col];
    }

#ifndef SWIG
    /**
     * Const access to the underlying representation.
     * Warning: This is unprotected.
     * @param i_row  The row number, zero based.
     * @param i_col  The column number, zero based.
     * @return Value of the element at i_row, i_col.
     */
    double operator() (unsigned i_row, unsigned i_col) const
    {
        return const_cast<TwoDArray&>(*this)(i_row, i_col);
    }
#endif


    /**
     * Non-const row major element access to the underlying representation.
     * Warning: This is protected, which comes at a cost.
     * @param i_row  The row number, zero based.
     * @param i_col  The column number, zero based.
     * @return Reference to element at i_row, i_col.
     */
    double& at (unsigned i_row, unsigned i_col)
    {
        if ((i_row >= n_rows) || (i_col >= n_cols))
        {
            throw std::out_of_range("Index out of bounds");
        }
        return (*this)(i_row, i_col);
    }

#ifndef SWIG
    /**
     * Const access to the underlying representation.
     * Warning: This is protected, which comes at a cost.
     * @param i_row  The row number, zero based.
     * @param i_col  The column number, zero based.
     * @return Value of the element at i_row, i_col.
     */
    double at (unsigned i_row, unsigned i_col) const
    {
        return const_cast<TwoDArray&>(*this).at(i_row, i_col);
    }
#endif


#ifndef SWIG
    /**
     * Create a 2D view into the array.
     * Note: the end row and end column are one past the end,
     * consistent with c++ iterators and ranges.
     * @param row_range  Rows at which view begins and ends.
     * @param col_range  Columns at which view begins and ends.
     */
    SubArrayView<TwoDArray, double> make_view (
        const SolverTypes::IndexPairT& row_range,
        const SolverTypes::IndexPairT& col_range)
    {
        return SubArrayView<TwoDArray, double>(*this, row_range, col_range);
    }

    /**
     * Create a view into a part of a row of the array.
     * Note: the end column is one past the end,
     * consistent with c++ iterators and ranges.
     * @param the_row    The row of the view.
     * @param col_range  Columns at which view begins and ends/
     */
    RowView<TwoDArray, double> make_view (
        unsigned the_row,
        const SolverTypes::IndexPairT& col_range)
    {
        return RowView<TwoDArray, double>(*this, the_row, col_range);
    }

    /**
     * Create a view into a part of a column of the array.
     * Note: the end row is one past the end,
     * consistent with c++ iterators and ranges.
     * @param row_range  Rows at which view begins and ends.
     * @param the_col    The column of the view.
     */
    ColumnView<TwoDArray, double> make_view (
        const SolverTypes::IndexPairT& row_range,
        unsigned the_col)
    {
        return ColumnView<TwoDArray, double>(*this, row_range, the_col);
    }
#endif


private:

    /**
     * The underlying representation.
     */
    DoubleVectorT repr; //!< trick_io(**)

    /**
     * The maximum number of elements in the array.
     */
    unsigned max_elems; //!< trick_units(--)

    /**
     * The number of rows currently in use.
     */
    unsigned n_rows; //!< trick_units(--)

    /**
     * The number of columns currently in use.
     */
    unsigned n_cols; //!< trick_units(--)


    /**
     * JEOD registration.
     */
    void register_contents ()
    {
        JEOD_REGISTER_NONEXPORTED_CLASS(TwoDArray);
        JEOD_REGISTER_CHECKPOINTABLE (this, repr);
    }

    /**
     * JEOD deregistration.
     */
    void deregister_contents ()
    {
        JEOD_DEREGISTER_CHECKPOINTABLE (this, repr);
    }
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
