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
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup GaussJackson
 * @{
 *
 * @file models/utils/integration/gauss_jackson/include/two_d_array.hh
 * Defines the template class er7_utils::TwoDArray, which implements an RAII
 * rectangular 2D array.
 */

/*
Purpose: ()
*/

#ifndef ER7_UTILS_TWO_D_ARRAY_HH
#define ER7_UTILS_TWO_D_ARRAY_HH

#include "er7_utils/interface/include/alloc.hh"
#include "er7_utils/interface/include/er7_class.hh"
#include "utils/sim_interface/include/config.hh"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace er7_utils
{

/**
 * RAII template class that implements a rectangular two dimensional array.
 * The implementation provides two special-purpose features that are needed by
 * some of the ER7 utilities and JEOD integrators. The rows of the array can
 * be rotated and downsampled.
 * @tparam  T  Type of each element of the array.
 */
template<typename T> class TwoDArray
{
public:
    /**
     * Default constructor.
     */
    TwoDArray() = default;

    /**
     * Copy constructor.
     * @param  src  Item to be copied.
     */
    TwoDArray(const TwoDArray<T> & src)
    {
        // Allocate new storage based on the source size.
        allocate(src.n, src.m);

        // Copy the source data.
        // Maintenance note:
        // Do not use memcpy to copy the whole data_array in one shot. This must
        // be done on a row by row basis because of rotation and downsampling.
        for(int ii = 0; ii < n; ++ii)
        {
            std::memcpy(row_array[ii], src.row_array[ii], m * sizeof(T));
        }
    }

    /**
     * Destructor.
     */
    ~TwoDArray()
    {
        deallocate_internal();
    }

    /**
     * Copy and swap assignment constructor.
     * @param  src  Item to be copied.
     */
    TwoDArray<T> & operator=(TwoDArray<T> src)
    {
        swap(*this, src);
        return *this;
    }

    /**
     * Const overloaded index operator.
     * @param  N  Row index.
     * @return Const pointer to the Nth row in the array.
     */
    const T * operator[](int N) const
    {
        return row_array[N];
    }

    /**
     * Non-const overloaded index operator.
     * @param  N  Row index.
     * @return Modifiable pointer to the Nth row in the array.
     */
    T * operator[](int N)
    {
        return row_array[N];
    }

    /**
     * Const overloaded function operator.
     * @param  N  Row index.
     * @param  M  Column index.
     * @return Const reference to the N,M element of the array.
     */
    const T & operator()(int N, int M) const
    {
        return row_array[N][M];
    }

    /**
     * Non-const overloaded function operator.
     * @param  N  Row index.
     * @param  M  Column index.
     * @return Reference to the N,M element of the array.
     */
    T & operator()(int N, int M)
    {
        return row_array[N][M];
    }

    /**
     * Const conversion operator to T const* const*.
     * @return Non-modifiable pointer to the array.
     */
    operator const T * const *() const
    {
        return row_array;
    }

    /**
     * Non-const conversion operator to T*const*.
     * @return Modifiable pointer to the array.
     */
    operator T * const *()
    {
        return row_array;
    }

    /**
     * Range-checked equivalent of const T* operator[](int N) const.
     * @param  N  Row index.
     * @return Const pointer to the Nth row in the array.
     * @throw  std::out_of_range  If @a N is an invalid index.
     */
    const T * at(int N) const
    {
        if((N < 0) || (N >= n))
        {
            JEOD_THROW(std::out_of_range("Index out of range"));
        }
        return row_array[N];
    }

    /**
     * Range-checked equivalent of T* operator[](int N).
     * @param  N  Row index.
     * @return Modifiable pointer to the Nth row in the array.
     * @throw  std::out_of_range  If @a N is an invalid index.
     */
    T * at(int N)
    {
        if((N < 0) || (N >= n))
        {
            JEOD_THROW(std::out_of_range("Index out of range"));
        }
        return row_array[N];
    }

    /**
     * Range-checked equivalent of const T& operator()(int N, int M) const.
     * @param  N  Row index.
     * @param  M  Column index.
     * @return Const reference to the N,M element of the array.
     * @throw  std::out_of_range  If @a N or @a M is an invalid index.
     */
    const T & at(int N, int M) const
    {
        if((N < 0) || (N >= n) || (M < 0) || (M >= m))
        {
            JEOD_THROW(std::out_of_range("Index out of range"));
        }
        return row_array[N][M];
    }

    /**
     * Range-checked equivalent of T& operator()(int N, int M).
     * @param  N  Row index.
     * @param  M  Column index.
     * @return Reference to the N,M element of the array.
     * @throw  std::out_of_range  If @a N or @a M is an invalid index.
     */
    T & at(int N, int M)
    {
        if((N < 0) || (N >= n) || (M < 0) || (M >= m))
        {
            JEOD_THROW(std::out_of_range("Index out of range"));
        }
        return row_array[N][M];
    }

    /**
     * Allocate the array.
     * @param  N  Number of rows in the array.
     * @param  M  Number of columns in the array.
     * @throw  std::domain_error  N and/or M won't fit in a signed int.
     */
    void allocate(std::size_t N, std::size_t M)
    {
        if((N > std::size_t(std::numeric_limits<int>::max())) || (M > std::size_t(std::numeric_limits<int>::max())))
        {
            JEOD_THROW(std::domain_error("Sizes too large"));
        }

        if(data_array != nullptr)
        {
            deallocate_internal();
        }

        n = static_cast<int>(N);
        m = static_cast<int>(M);
        allocate_internal();

        for(int ii = 0; ii < n; ++ii)
        {
            row_array[ii] = data_array + ii * m;
        }
    }

    /**
     * Rotate elements 0 to limit downward, with array element 0 moved to
     * array element limit.
     * @param limit  Index of last element participating in the rotation.
     * @throw  std::out_of_range  If @a limit is an invalid index.
     */
    void rotate_down(int limit)
    {
        if((limit < 0) || (limit >= n))
        {
            JEOD_THROW(std::out_of_range("Index out of range"));
        }

        T * zeroth = row_array[0];
        for(int ii = 0; ii < limit; ++ii)
        {
            row_array[ii] = row_array[ii + 1];
        }
        row_array[limit] = zeroth;
    }

    /**
     * Rotate elements 0 to limit upward, with array element limit moved to
     * array element 0.
     * @param limit  Index of last element participating in the rotation.
     * @throw  std::out_of_range  If @a limit is an invalid index.
     */
    void rotate_up(int limit)
    {
        if((limit < 0) || (limit >= n))
        {
            JEOD_THROW(std::out_of_range("Index out of range"));
        }

        T * last = row_array[limit];
        for(int ii = limit; ii > 0; --ii)
        {
            row_array[ii] = row_array[ii - 1];
        }
        row_array[0] = last;
    }

    /**
     * Downsample the array by swapping pointers.
     * @param limit  Number of usable rows after downsample.
     * @throw  std::out_of_range  If @a limit represents an invalid index.
     */
    void downsample(int limit)
    {
        if((limit < 0) || (2 * (limit - 1) >= n))
        {
            JEOD_THROW(std::out_of_range("Index out of range"));
        }

        for(int ii = 1; ii < limit; ++ii)
        {
            std::swap(row_array[ii], row_array[2 * ii]);
        }
    }

    /**
     * Swap the contents of *this with the other.
     * @param other Other array.
     */
    void swap(TwoDArray<T> & other)
    {
        std::swap(n, other.n);
        std::swap(m, other.m);
        std::swap(data_array, other.data_array);
        std::swap(row_array, other.row_array);
    }

    /**
     * Swap the contents of the two provided arrays.
     * @param first  First array.
     * @param second Second array.
     */
    friend void swap(TwoDArray<T> & first, TwoDArray<T> & second)
    {
        first.swap(second);
    }

protected:
    // Member data.
    // These are protected rather than private so that Trick can see them.

    /**
     * The number of rows in the array.
     */
    int n{}; //!< trick_units(--)

    /**
     * The number of columns in the array.
     */
    int m{}; //!< trick_units(--)

    /**
     * The array data, as an NxM array of T.
     */
    T * data_array{}; //!< trick_units(--)

    /**
     * The rows in the array.
     */
    T ** row_array{}; //!< trick_units(--)

private:
    /**
     * Allocate memory for the array.
     */
    void allocate_internal()
    {
        data_array = er7_utils::alloc::allocate_array<T>(n * m);
        row_array = er7_utils::alloc::allocate_array<T *>(n);
    }

    /**
     * Deallocate memory for the array.
     */
    void deallocate_internal()
    {
        er7_utils::alloc::deallocate_array<T>(data_array);
        er7_utils::alloc::deallocate_array<T *>(row_array);
    }
};

/**
 * 2D array, specialized for doubles.
 */
class DoubleTwoDArray : public TwoDArray<double>
{
    ER7_UTILS_MAKE_SIM_INTERFACES(DoubleTwoDArray)
};

} // namespace er7_utils

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
