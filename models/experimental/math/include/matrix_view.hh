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
 * Defines the class template MatrixView.
 */

/*
Purpose: ()
SWIG: (No)
*/

#ifndef JEOD_MATRIX_VIEW_HH
#define JEOD_MATRIX_VIEW_HH

#include "forward_view.hh"
#include "solver_types.hh"

#include <stdexcept>

//! Namespace jeod
namespace jeod
{

/**
 * Encapsulates a collection of values of the underlying type that can
 * conceptually be indexed by a pair of integers (unsigned).
 *
 * Future: For a 1xN or Nx1 matrix, add a vector-like view.
 *
 * Future: Add python-like referencing, where SomeView[-1][-1] references
 * the last value.
 *
 * @tparam ElemType The matrix element type, assumed to be a primitive type.
 */
template<typename ElemType, typename RefType> class MatrixView
{
public:
    /**
     * Non-default constructor.
     * @param n_rows_in  The number of rows in the view.
     * @param n_cols_in  The number of columns in the view.
     */
    MatrixView(unsigned n_rows_in, unsigned n_cols_in)
        : n_rows(n_rows_in),
          n_cols(n_cols_in)
    {
    }

    /**
     * Destructor. There's nothing to destruct here.
     */
    virtual ~MatrixView() = default;

    /**
     * Copy assignment.
     * The default implementation does an item-for-item copy,
     * which might be suboptimal (e.g., memcpy might be faster).
     * @param src  The view whose data are to be copied into this.
     * @throw std::length_error if this view and the source view are
     * incompatibly sized.
     */
    virtual MatrixView & operator=(MatrixView & src)
    {
        if((src.n_rows != n_rows) || (src.n_cols != n_cols))
        {
            JEOD_THROW(std::length_error("Incompatible sizes"));
        }

        // To make cppcheck happy
        n_rows = src.n_rows;
        n_cols = src.n_cols;

        for(unsigned ii = 0; ii < n_rows; ++ii)
        {
            for(unsigned jj = 0; jj < n_cols; ++jj)
            {
                (*this)(ii, jj) = src(ii, jj);
            }
        }

        return *this;
    }

    /**
     * Non-const function call operator, as an index operator.
     * @param i_row Row number of element to be indexed.
     * @param i_col Column number of element to be indexed.
     * @return Reference to the indexed value.
     */
    virtual RefType operator()(unsigned int i_row, unsigned int i_col) = 0;

    /**
     * Const function call operator, as an index operator.
     * @param i_row Row number of element to be indexed.
     * @param i_col Column number of element to be indexed.
     * @return The indexed value.
     */
    virtual ElemType operator()(unsigned int i_row, unsigned int i_col) const
    {
        return const_cast<MatrixView &>(*this)(i_row, i_col);
    }

    /**
     * Non-const safe (bounds-checked) index operator.
     * @param i_row Row number of element to be indexed.
     * @param i_col Column number of element to be indexed.
     * @return Reference to the indexed value.
     * @throw std::out_of_range if index is outside the range of the view.
     */
    virtual RefType at(unsigned int i_row, unsigned int i_col)
    {
        if((i_row >= n_rows) || (i_col >= n_cols))
        {
            JEOD_THROW(std::out_of_range("Illegal element access"));
        }
        return (*this)(i_row, i_col);
    }

    /**
     * Const safe (bounds-checked) index operator.
     * @param i_row Row number of element to be indexed.
     * @param i_col Column number of element to be indexed.
     * @return The indexed value.
     * @throw std::out_of_range if index is outside the range of the view.
     */
    virtual ElemType at(unsigned int i_row, unsigned int i_col) const
    {
        return const_cast<MatrixView &>(*this).at(i_row, i_col);
    }

    /**
     * Size function.
     * @return The number of rows and columns in the view, as a pair.
     */
    virtual SolverTypes::IndexPairT size() const
    {
        return SolverTypes::IndexPairT(n_rows, n_cols);
    }

    /**
     * Create a view of a NxM submatrix of this view.
     * @param row_range The begin and end rows in the to-be-created view.
     * @param col_range The begin and end columns in the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub array.
     */
    SubArrayView<MatrixView<ElemType, RefType>, ElemType, RefType> make_view(const SolverTypes::IndexPairT & row_range,
                                                                             const SolverTypes::IndexPairT & col_range);

    /**
     * Create a view of a NxM submatrix of this view.
     * @param row_range The begin and end rows in the to-be-created view.
     * @param col_range The begin and end columns in the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub array.
     */
    SubArrayView<const MatrixView<ElemType, RefType>, ElemType, ElemType> make_view(
        const SolverTypes::IndexPairT & row_range, const SolverTypes::IndexPairT & col_range) const;

    /**
     * Create a view of a portion of a row of this view.
     * @param the_row The row referenced by the to-be-created view.
     * @param col_range The begin and end columns in the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub vector.
     */
    RowView<MatrixView<ElemType, RefType>, ElemType, RefType> make_view(unsigned the_row,
                                                                        const SolverTypes::IndexPairT & col_range);

    /**
     * Create a view of a portion of a row of this view.
     * @param the_row The row referenced by the to-be-created view.
     * @param col_range The begin and end columns in the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub vector.
     */
    RowView<const MatrixView<ElemType, RefType>, ElemType, ElemType> make_view(
        unsigned the_row, const SolverTypes::IndexPairT & col_range) const;

    /**
     * Create a view of a portion of a column of this view.
     * @param row_range The begin and end rows in the to-be-created view.
     * @param the_col The column referenced by the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub vector.
     */
    ColumnView<MatrixView<ElemType, RefType>, ElemType, RefType> make_view(const SolverTypes::IndexPairT & row_range,
                                                                           unsigned the_col);

    /**
     * Create a view of a portion of a column of this view.
     * @param row_range The begin and end rows in the to-be-created view.
     * @param the_col The column referenced by the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub vector.
     */
    ColumnView<const MatrixView<ElemType, RefType>, ElemType, ElemType> make_view(
        const SolverTypes::IndexPairT & row_range, unsigned the_col) const;

private:
    /**
     * The number of rows in the view.
     */
    unsigned n_rows; //!< trick_units(--)

    /**
     * The number of columns in the view.
     */
    unsigned n_cols; //!< trick_units(--)
};

} // namespace jeod

#include "column_view.hh"
#include "row_view.hh"
#include "sub_array_view.hh"

//! Namespace jeod
namespace jeod
{

template<typename ElemType, typename RefType>
inline SubArrayView<MatrixView<ElemType, RefType>, ElemType, RefType> MatrixView<ElemType, RefType>::make_view(
    const SolverTypes::IndexPairT & row_range, const SolverTypes::IndexPairT & col_range)
{
    return SubArrayView<MatrixView<ElemType, RefType>, ElemType, RefType>(*this, row_range, col_range);
}

template<typename ElemType, typename RefType>
inline SubArrayView<const MatrixView<ElemType, RefType>, ElemType, ElemType> MatrixView<ElemType, RefType>::make_view(
    const SolverTypes::IndexPairT & row_range, const SolverTypes::IndexPairT & col_range) const
{
    return SubArrayView<const MatrixView<ElemType, RefType>, ElemType, ElemType>(*this, row_range, col_range);
}

template<typename ElemType, typename RefType>
inline RowView<MatrixView<ElemType, RefType>, ElemType, RefType> MatrixView<ElemType, RefType>::make_view(
    unsigned the_row, const SolverTypes::IndexPairT & col_range)
{
    return RowView<MatrixView<ElemType, RefType>, ElemType, RefType>(*this, the_row, col_range);
}

template<typename ElemType, typename RefType>
inline RowView<const MatrixView<ElemType, RefType>, ElemType, ElemType> MatrixView<ElemType, RefType>::make_view(
    unsigned the_row, const SolverTypes::IndexPairT & col_range) const
{
    return RowView<const MatrixView<ElemType, RefType>, ElemType, ElemType>(*this, the_row, col_range);
}

template<typename ElemType, typename RefType>
inline ColumnView<MatrixView<ElemType, RefType>, ElemType, RefType> MatrixView<ElemType, RefType>::make_view(
    const SolverTypes::IndexPairT & row_range, unsigned the_col)
{
    return ColumnView<MatrixView<ElemType, RefType>, ElemType, RefType>(*this, row_range, the_col);
}

template<typename ElemType, typename RefType>
inline ColumnView<const MatrixView<ElemType, RefType>, ElemType, ElemType> MatrixView<ElemType, RefType>::make_view(
    const SolverTypes::IndexPairT & row_range, unsigned the_col) const
{
    return ColumnView<const MatrixView<ElemType, RefType>, ElemType, ElemType>(*this, row_range, the_col);
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
