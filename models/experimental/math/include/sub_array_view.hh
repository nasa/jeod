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
 * Defines the class template SubArrayView.
 */

/*
Purpose: ()
SWIG: (No)
*/

#ifndef JEOD_SUB_ARRAY_VIEW_HH
#define JEOD_SUB_ARRAY_VIEW_HH

#include "forward_view.hh"
#include "matrix_view.hh"
#include "solver_types.hh"

#include <tuple>

//! Namespace jeod
namespace jeod
{

/**
 * Represents a subarray view into an array.
 * The function call operator provides unprotected access to an element
 * of the subarray. The member function at() ensures that the indexed
 * element is valid.
 */
template<class ParentType, typename ElemType, typename RefType>
class SubArrayView : public MatrixView<ElemType, RefType>
{
public:
    /**
     * Non-default constructor.
     * @param array_in   The array for which a view is to be constructed.
     * @param row_range  The topmost and bottommost row of the view, plus one.
     * @param col_range  The leftmost and rightmost column of the view, plus one.
     * @throw std::out_of_range if sub-array bounds are invalid.
     */
    SubArrayView(ParentType & array_in,
                 const SolverTypes::IndexPairT & row_range,
                 const SolverTypes::IndexPairT & col_range)
        : SubArrayView(array_in, row_range.first, row_range.second, col_range.first, col_range.second)
    {
    }

    /**
     * Non-default constructor.
     * @param array_in   The array for which a view is to be constructed.
     * @param beg_row_in  The topmost row of the view.
     * @param end_row_in  The bottommost row of the view, plus one.
     * @param beg_col_in  The leftmost column of the view.
     * @param end_col_in  The rightmost column of the view, plus one.
     * @throw std::out_of_range if sub-array bounds are invalid.
     */
    SubArrayView(
        ParentType & array_in, unsigned beg_row_in, unsigned end_row_in, unsigned beg_col_in, unsigned end_col_in)
        : MatrixView<ElemType, RefType>(end_row_in - beg_row_in, end_col_in - beg_col_in),
          array(array_in),
          beg_row(beg_row_in),
          beg_col(beg_col_in)
    {
        const auto & array_size = array_in.size();
        if((end_row_in < beg_row_in) || (end_row_in > array_size.first) || (end_col_in < beg_col_in) ||
           (end_col_in > array_size.second))
        {
            JEOD_THROW(std::out_of_range("Attempt to create illegal SubArrayView"));
        }
    }

    /**
     * Get a modifiable (L-value) reference to the i_row, i_col element
     * of the sub-array. No bounds checking is performed.
     * @param i_row  The row number, zero based.
     * @param i_col  The column number, zero based.
     * @return L-value reference to the specified element.
     */
    virtual RefType operator()(unsigned i_row, unsigned i_col) final
    {
        return array(beg_row + i_row, beg_col + i_col);
    }

private:
    /**
     * The array into which a view is being presented.
     */
    ParentType & array; //!< trick_units(--)

    /**
     * The topmost row of the view.
     */
    unsigned beg_row{}; //!< trick_units(--)

    /**
     * The leftmost column of the view.
     */
    unsigned beg_col{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
