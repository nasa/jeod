/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/force.cc
 * Define force model member functions.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((force.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/force.hh"

//! Namespace jeod
namespace jeod
{

/**
 * CollectForce constructor that encapsulates a Force.
 * Note that this performs a shallow copy by intent.
 * \param[in,out] source_force Force to encapsulate
 */
CollectForce::CollectForce(Force & source_force)
    : active(&source_force.active),
      force(source_force.force)
{
}

/**
 * CollectForce constructor that encapsulates a C-style 3-vector.
 * Note that the new CollectForce's force *is* the force_3vec.
 * \param[in,out] force_3vec Force vector to encapsulate\n Units: N
 */
CollectForce::CollectForce(double force_3vec[3])
    : force(force_3vec)
{
}

/**
 * CollectForce constructor that encapsulates another CollectForce.
 * Note that this performs a shallow copy by intent.
 * \param[in,out] source_force Force to encapsulate
 */
CollectForce::CollectForce(CollectForce & source_force)
    : active(source_force.active),
      force(source_force.force)
{
}

/**
 * CInterfaceForce constructor for use with C force array.
 * Note that the new CInterfaceForce's force *is* the force_3vec.
 * \param[in,out] force_3vec Force vector to encapsulate\n Units: N
 */
CInterfaceForce::CInterfaceForce(double * force_3vec)
    : CollectForce()
{
    active = JEOD_ALLOC_PRIM_OBJECT(bool, (true));
    force = force_3vec;
}

/**
 * CInterfaceForce destructor; frees 'active' but not the force.
 */
CInterfaceForce::~CInterfaceForce()
{
    JEOD_DELETE_OBJECT(active);
}

/**
 * Create a shallow copy of a Force.
 * Note that the new CollectForce refers to the Force's
 * active flag and force array.
 * @return Constructed CollectForce
 * \param[in,out] source_force Force object to encapsulate
 */
CollectForce * CollectForce::create(Force & source_force)
{
    return JEOD_ALLOC_CLASS_OBJECT(CollectForce, (source_force));
}

/**
 * Create a shallow copy of a Force.
 * Note that the new CollectForce refers to the Force's
 * active flag and force array.
 * @return Constructed CollectForce
 * \param[in,out] source_force Force object to encapsulate
 */
CollectForce * CollectForce::create(Force * source_force)
{
    return CollectForce::create(*source_force);
}

/**
 * Create a CollectForce whose force is the specified array.
 * Note that the created instance is actually a CInterfaceForce.
 * @return Constructed CollectForce
 * \param[in,out] force_3vec Force vector to encapsulate\n Units: N
 */
CollectForce * CollectForce::create(double * force_3vec)
{
    return JEOD_ALLOC_CLASS_OBJECT(CollectForce, (force_3vec));
}

/**
 * Create a shallow copy of a CollectForce.
 * Note that both the source and new CollectForces refer
 * to the same active flag and force array.
 * @return Constructed CollectForce
 * \param[in,out] source_force Force to copy
 */
CollectForce * CollectForce::create(CollectForce & source_force)
{
    return JEOD_ALLOC_CLASS_OBJECT(CollectForce, (source_force));
}

/**
 * Create a shallow copy of a CollectForce.
 * Note that both the source and new CollectForces refer
 * to the same active flag and force array.
 * @return Constructed CollectForce
 * \param[in,out] source_force Force to copy
 */
CollectForce * CollectForce::create(CollectForce * source_force)
{
    return CollectForce::create(*source_force);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
