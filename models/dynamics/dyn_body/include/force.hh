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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/include/force.hh
 * Define the JEOD force model.
 */

/*******************************************************************************
Purpose:
  ()

Library dependencies:
  ((../src/force.cc))



*******************************************************************************/

#ifndef JEOD_FORCE_HH
#define JEOD_FORCE_HH

//! Namespace jeod
namespace jeod
{

/**
 * A Force represents a Newtonian force that acts on a DynBody.
 * The class encapsulates an active flag and a 3-vector that contains the
 * force components. Forces are collected in one of a DynBody object's
 * force collection STL vectors. The force vector is expressed in the
 * structural frame of that DynBody object.
 *
 * The Force class is the recommended mechanism for representing forces in JEOD.
 * While 3-vectors can also be collected into a collect STL vector, theee is
 * is no way to turn off these collected 3-vectors. Even worse, there is no way
 * to tell whether a collected 3-vector does indeed represent a force -- or
 * even if it is a 3-vector. In comparison, Force objects can be turned on and
 * off, and more importantly, they are type-safe.
 */
class Force
{
public:
    // Member functions
    Force() = default;
    virtual ~Force() = default;
    Force(const Force &) = delete;
    Force & operator=(const Force &) = delete;

    // Member data

    /**
     * Is this force active?
     */
    bool active{true}; //!< trick_units(--)

    /**
     * Force vector
     */
    double force[3]{}; //!< trick_units(N)

    // Operators; listed out-of-order due to Trick07 ICG.

    // Subscript operator: Access force elements as an lvalue.
    double & operator[](const unsigned int index);
    double operator[](const unsigned int index) const;
};

/**
 * A CollectForce represents a collected force that acts on a vehicle.
 * The BodyForceCollect class contains STL vectors that in turn contain
 * CollectForce pointers. These vectors are populated via the Trick vcollect
 * mechanism. A Trick simulation issues vcollect statements such as
 * \code
 * vcollect vehicle.body.collect.collect_XXX_forc CollectForce::create {
 *    vehicle.force_model1.force,
 *    vehicle.force_model2.force
 * };
 * \endcode
 * This invokes the appropriate CollectForce create method on each listed
 * element.
 *
 * CollectForces should not be used in model code to represent forces.
 * Use the Force class instead.
 */
class CollectForce
{
public:
    // Static member functions

    /* Factory constructors */
    static CollectForce * create(double * vec);
    static CollectForce * create(Force & force);
    static CollectForce * create(CollectForce & force);

#ifndef SWIG

    static CollectForce * create(Force * force);
    static CollectForce * create(CollectForce * force);

#endif

    // Constructors
    CollectForce() = default;
    explicit CollectForce(double vec[3]);
    explicit CollectForce(Force &);
    explicit CollectForce(CollectForce &);

    virtual ~CollectForce() = default;
    CollectForce(const CollectForce &) = delete;
    CollectForce & operator=(const CollectForce &) = delete;

    // Is the force active?
    bool is_active() const;

    // Subscript operator: Access force elements as an lvalue.
    double & operator[](const unsigned int index);
    double operator[](const unsigned int index) const;

    inline bool operator==(const CollectForce & other)
    {
        return (force == other.force);
    }

    // Member data
    // That these are public is deprecated.

    /**
     * Is this force active?
     */
    bool * active{}; //!< trick_units(--)

    /**
     * Force vector
     */
    double * force{}; //!< trick_units(N)
};

/**
 * This class is deprecated.
 */
class CInterfaceForce : public CollectForce
{
public:
    // Member functions.

    // Constructors.
    CInterfaceForce() = default;
    explicit CInterfaceForce(double * vec);

    // Destructor
    ~CInterfaceForce() override;

    // Member data: This class adds no data to CollectForce.

    CInterfaceForce(const CInterfaceForce &) = delete;
    CInterfaceForce & operator=(const CInterfaceForce &) = delete;
};

} // namespace jeod

#include "force_inline.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
