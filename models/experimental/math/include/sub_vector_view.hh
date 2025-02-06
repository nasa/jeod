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
 * Defines the class template SubVectorView.
 */

/*
Purpose: ()
SWIG: (No)
*/


#ifndef JEOD_SUB_VECTOR_VIEW_HH
#define JEOD_SUB_VECTOR_VIEW_HH


#ifndef SWIG

#include "forward_view.hh"
#include "vector_view.hh"
#include "solver_types.hh"

#include <stdexcept>


//! Namespace jeod 
namespace jeod {

/**
 * Provides a view into a part of a vector.
 * @tparam UnderlyingType The type of the underlying object.
 *   This must, at a minimum, provide an index operator and a size method.
 * @tparam ElemType The type of the elements held by the underlying object.
 *   This is assumed to be a primitive type.
 * @tparam RefType      Nominally, a reference to ElemType.
 *   This should be ElemType in the case of a const SubVectorView so
 *   as to preclude alterations of the underlying object.
 * @tparam BaseRefType  Also nominally a reference to ElemType.
 *   This should also be ElemType in the case of a const SubVectorView so
 *   as to preclude alterations of the underlying object.
 */
template<
    class UnderlyingType,
    typename ElemType,
    typename RefType,
    typename BaseRefType>
class SubVectorView : public VectorView<ElemType, BaseRefType>
{
public:

    /**
     * A const-correct SubVectorView must not provide any operations that
     * modify the underlying object.
     */
    typedef SubVectorView<const UnderlyingType, ElemType, ElemType, ElemType>
        SubVectorViewToConst;


    /**
     * Non-default constructor.
     * @param obj_in  The object that underlies this view.
     * @param range  A begin, end pair that denote the extent of the view.
     *   The begin value corresponds to this view's zeroth element.
     *   The end value is one past the last element indexed by this view.
     *   To be valid, the end value must not be less than the begin value
     *   and must not be greater than the underlying vector's size.
     * @throw std::out_of_range if the range is invalid.
     */
    SubVectorView(
        UnderlyingType& obj_in,
        const SolverTypes::IndexPairT& range)
    :
        VectorView<ElemType, BaseRefType>(range.second-range.first),
        obj(obj_in),
        beg_elem(range.first)
    {
        if ((range.second <  range.first) ||
            (range.second >  obj.size()))
        {
            throw std::out_of_range(
                "Attempt to create an invalid slice of a vector");
        }
    }


    /**
     * Conversion operator to the equivalent const-correct view.
     * @return const SubVectorView that does not allow the underlying
     * object to be modified.
     */
    operator const SubVectorViewToConst& () const
    {
        return *reinterpret_cast<const SubVectorViewToConst*> (this);
    }


    /**
     * Non-const index operator.
     * @param i_elem View element to be indexed.
     * @return Reference to the indexed value of the view, which is the
     *   beg_elem+i_elem element of the underlying std::vector.
     */
    virtual RefType operator[] (unsigned i_elem) override
    {
        return obj[beg_elem+i_elem];
    }


protected:

    /**
     * The underlying object.
     */
    UnderlyingType& obj; //!< trick_units(--)

    /**
     * The vector element that corresponds to the zeroth view element.
     */
    const unsigned beg_elem; //!< trick_units(--)
};

} // End JEOD namespace

#endif

#endif


/**
 * @}
 * @}
 * @}
 */
