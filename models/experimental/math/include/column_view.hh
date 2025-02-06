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
 * Defines the class template ColumnView.
 */

/*
Purpose: ()
SWIG: (No)
*/


#ifndef JEOD_ARRAY_COLUMN_VIEW_HH
#define JEOD_ARRAY_COLUMN_VIEW_HH


#include "forward_view.hh"
#include "vector_view.hh"
#include "solver_types.hh"

#include <stdexcept>

//! Namespace jeod
namespace jeod {

/**
 * Represents a view into a column of an array.
 * The index operator provides unprotected access to an element
 * of the view. The member function at() ensures that the indexed
 * element is valid.
 * @tparam ArrayType The type of the underlying object.
 *   This must, at a minimum, provide an function call operator that
 *   accesses array elements and a size method that returns a pair.
 * @tparam ElemType The type of the elements held by the underlying object.
 *   This is assumed to be a primitive type.
 */
template <class ArrayType, typename ElemType, typename RefType>
class ColumnView : public VectorView<ElemType, RefType>
{
public:

    /**
     * Non-default constructor.
     * @note This can create a zero-sized view.
     * @param array_in  The 2D array that underlies this view.
     * @param row_range  A begin, end pair that denote the extent of the
     *   rows in this view.
     *   The begin value corresponds to this view's zeroth element.
     *   The end value is one past the last element indexed by this view.
     *   To be valid, the end value must not be less than the begin value
     *   and must not be greater than the underlying vector's size.
     * @param col_in  The column of the array to be represented by this view.
     *   This must reference a valid column of the underlying array.
     * @throw std::out_of_range if the range is invalid.
     */
    ColumnView (
        ArrayType& array_in,
        const SolverTypes::IndexPairT& row_range,
        unsigned col_in)
    :
        VectorView<ElemType>(row_range.second-row_range.first),
        array(array_in),
        the_col(col_in),
        beg_row(row_range.first)
    {
        const auto& array_size = array.size();
        if ((the_col          >= array_size.second) ||
            (row_range.second <  row_range.first) ||
            (row_range.second >  array_size.first))
        {
            throw std::out_of_range(
                "Attempt to create an invalid column view into an array");
        }
    }

    /**
     * Non-const index operator.
     * @param ielem View element to be indexed.
     * @return Reference to the indexed value of the view, which is the
     *   beg_elem+ielem element of the underlying std::vector.
     */
    RefType operator[] (unsigned ielem)
    {
        return array(beg_row+ielem, the_col);
    }

protected:

    /**
     * The underlying array.
     */
    ArrayType& array; //!< trick_units(--)


    /**
     * The array column encapsulated by this view.
     */
    const unsigned the_col; //!< trick_units(--)

    /**
     * The array row that corresponds to the zeroth view element.
     */
    const unsigned beg_row; //!< trick_units(--)
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
