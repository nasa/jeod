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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Defines the class BodyWrenchCollect.
 */

/*
Purpose: ()

Library dependencies:
  ((../src/body_wrench_collect.cc))
*/

#ifndef JEOD_BODY_WRENCH_COLLECT_HH
#define JEOD_BODY_WRENCH_COLLECT_HH

#include "wrench.hh"

#include "utils/container/include/pointer_vector.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Serves as the collection point for wrenches that act on a vehicle.
 * This is a simple class that is tightly coupled with the
 * StructureIntegratedDynBody class. This latter class contains (has-a)
 * a BodyWrenchCollect data member.
 *
 * The Trick vcollect mechanism (or a similar mechanism in a non-Trick sim)
 * pushes pointers to the individual wrenches onto the various collection
 * member of a BodyWrenchCollect.  StructureIntegratedDynBody members cumulate
 * these collected wrenches to form the total wrench acting on the vehicle.
 */
class BodyWrenchCollect
{
public:
    // Member data

    /**
     * Vector of effector wrenches.
     * The effector wrenches are collected into the vector at the
     * S_define level via
     &
     *     vcollect containing_body.effector_wrench_collection.collect_wrench {
     *         pointer_to_wrench1,
     *         ...
     *         pointer_to_wrench_n
     *     };
     *
     * The vector of collected wrenches are processed by the containing body's
     * collect_forces_and_torques member function.
     */
    JeodPointerVector<Wrench>::type collect_wrench; //!< trick_io(**)

    // Member functions

    // Member functions

    /**
     * Default constructor.
     */
    BodyWrenchCollect();

    /**
     * Destructor.
     */
    ~BodyWrenchCollect();

    // The copy constructor and copy assignment operator are deleted
    // to avoid erroneous copies.
    BodyWrenchCollect(const BodyWrenchCollect &) = delete;
    BodyWrenchCollect & operator=(const BodyWrenchCollect &) = delete;

    /**
     * Accumulate the collected wrenches.
     * @param sum  Wrench into which the accumulated sum is to be placed.
     * The summation is about sum.point.
     * @return Reference to the input wrench.
     */
    Wrench & accumulate(Wrench & sum) const
    {
        sum.accumulate(collect_wrench);
        return sum;
    }

    /**
     * Accumulate the collected wrenches.
     * @param point  Point about which summation is to be performed.
     * @param sum    Wrench into which the accumulated sum is to be placed.
     * @return Reference to the input wrench.
     */
    Wrench & accumulate(const double point[3], Wrench & sum) const
    {
        sum.set_point(point);
        return accumulate(sum);
    }
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
