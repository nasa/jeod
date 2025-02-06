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
 * Define the class template VectorView.
 */

/*
Purpose: ()
SWIG: (No)
*/


#ifndef JEOD_VECTOR_VIEW_HH
#define JEOD_VECTOR_VIEW_HH


#include "forward_view.hh"
#include "solver_types.hh"

#include <cstddef>
#include <stdexcept>
#include <type_traits>


//! Namespace jeod 
namespace jeod {

/**
 * Provides a view into an object that can conceptually be indexed by a single
 * integer (currently unsigned). For example, part of a one dimensional array
 * can be encapsulated as a view, as can part of a specific row or column
 * of a two dimensional array.
 *
 * This class template is not instantiable. An instantiable derived class must
 * at a minimum implement the index operator.
 *
 * @warning Destructing the object that underlies a view invalidates the view.
 * Changing the shape or size of the underlying object may invalidate the view.
 * Referencing into an invalidated view is undefined behavior.
 *
 * @todo Add python-like referencing, where some_view[-1] references the last
 * value in the view, some_view[-2] references the penultimate value, etc.
 *
 * @todo Add an unsafe copy constructor in the case that the source does
 * not provide a size() member.
 *
 * @todo Consider replacing use of unsigned with std::size_t.
 *
 * @tparam ElemType The type of the elements held by the underlying object.
 *   This is assumed to be a primitive type.
 */
template<typename ElemType, typename RefType>
class VectorView
{
public:


    /**
     * Non-default constructor.
     * @param n_elems_in  The number of elements in the view.
     */
    VectorView (
        unsigned n_elems_in)
    :
        n_elems(n_elems_in)
    { }

    /**
     * Destructor. There's nothing to destruct here.
     */
    virtual ~VectorView() = default;


    /**
     * Non-const index operator.
     * @param i_elem Element to be indexed.
     * @return Reference to the indexed value.
     */
    virtual RefType operator[] (unsigned i_elem) = 0;


    /**
     * Copy assignment from an object that implements size().
     * The default implementation does an item-for-item copy,
     * which might be suboptimal (e.g., memcpy might be faster).
     * @param src  The view whose data are to be copied into this.
     * @throw std::length_error if this view and the source view are
     * incompatibly sized.
     */
    template<typename SourceType, typename T = VectorView>
    typename std::enable_if<! std::is_reference<RefType>::value, T&>::type
    operator= (const SourceType& src)
    {
        if (src.size() != n_elems)
        {
            throw std::length_error ("Incompatible sizes");
        }

        for (unsigned ii = 0; ii < n_elems; ++ii)
        {
            (*this)[ii] = src[ii];
        }

        return *this;
    }


    /**
     * Copy assignment from a primitive array.
     * The default implementation does an item-for-item copy,
     * which might be suboptimal (e.g., memcpy might be faster).
     * @param src  The view whose data are to be copied into this.
     * @throw std::length_error if this view and the source view are
     * incompatibly sized.
     */
    template<std::size_t N, typename T = VectorView>
    typename std::enable_if<! std::is_reference<RefType>::value, T&>::type
    operator= (const ElemType src[N])
    {
        if (N != n_elems)
        {
            throw std::length_error ("Incompatible sizes");
        }

        for (unsigned ii = 0; ii < n_elems; ++ii)
        {
            (*this)[ii] = src[ii];
        }

        return *this;
    }


    /**
     * Const index operator.
     * @param i_elem Element to be indexed.
     * @return The indexed value.
     */
    virtual ElemType operator[] (unsigned i_elem) const
    {
        return const_cast<VectorView&>(*this)[i_elem];
    }

    /**
     * Safe (bounds-checked) index operator.
     * @param i_elem Element to be indexed.
     * @return Reference to the indexed value.
     * @throw std::out_of_range if index is outside the range of the view.
     */
    virtual RefType at (unsigned i_elem)
    {
        if (i_elem >= n_elems)
        {
            throw std::out_of_range ("Illegal element access");
        }

        return (*this)[i_elem];
    }

    /**
     * Const safe (bounds-checked) index operator.
     * @param i_elem Element to be indexed.
     * @return The indexed value.
     * @throw std::out_of_range if index is outside the range of the view.
     */
    virtual ElemType at (unsigned i_elem) const
    {
        return const_cast<VectorView&>(*this).at(i_elem);
    }

    /**
     * Get the number of data elements encapsulated by this view.
     * @return View size.
     */
    std::size_t size() const
    {
        return n_elems;
    }

    /**
     * Create a view into this view.
     * @param range The begin and end elements in the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub vector.
     */
    SubVectorView<
        VectorView<ElemType, RefType>,
        ElemType,
        RefType,
        RefType>
    make_view (const SolverTypes::IndexPairT& range);

    /**
     * Create a view into this view.
     * @param range The begin and end elements in the to-be-created view.
     * @note The 'end' in the above has the canonical range meaning of the last
     * item plus one.
     * @return The created view, as a sub vector.
     */
    SubVectorView<
        const VectorView<ElemType, RefType>,
        ElemType,
        ElemType,
        RefType>
    make_view (const SolverTypes::IndexPairT& range) const;



protected:

    /**
     * The number of elements in the view.
     */
    const unsigned n_elems; //!< trick_units(--)
};

} // End JEOD namespace


#include "sub_vector_view.hh"


//! Namespace jeod 
namespace jeod {

template<typename ElemType, typename RefType>
inline
SubVectorView<
    VectorView<ElemType, RefType>,
    ElemType,
    RefType,
    RefType>
VectorView<ElemType,RefType>::make_view (
    const SolverTypes::IndexPairT& range)
{
    return SubVectorView<
        VectorView<ElemType, RefType>,
        ElemType,
        RefType,
        RefType> (*this, range);
}

template<typename ElemType, typename RefType>
inline
SubVectorView<
    const VectorView<ElemType, RefType>,
    ElemType,
    ElemType,
    RefType>
VectorView<ElemType, RefType>::make_view (
    const SolverTypes::IndexPairT& range) const
{
    return SubVectorView<
        const VectorView<ElemType, RefType>,
        ElemType,
        ElemType,
        RefType> (*this, range);
}


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
