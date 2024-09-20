/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/torque.cc
 * Define torque model member functions.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((torque.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/torque.hh"

//! Namespace jeod
namespace jeod
{

/**
 * CollectTorque constructor that encapsulates a Torque.
 * Note that this performs a shallow copy by intent.
 * \param[in,out] source_torque Torque to encapsulate
 */
CollectTorque::CollectTorque(Torque & source_torque)
    : active(&source_torque.active),
      torque(source_torque.torque)
{
}

/**
 * CollectTorque constructor that encapsulates a C-style 3-vector.
 * Note that the new CollectTorque's torque *is* the torque_3vec.
 * \param[in,out] torque_3vec Torque vector to encapsulate\n Units: NM
 */
CollectTorque::CollectTorque(double torque_3vec[3])
    : torque(torque_3vec)
{
}

/**
 * CollectTorque constructor that encapsulates another CollectTorque.
 * Note that this performs a shallow copy by intent.
 * \param[in,out] source_torque Torque to encapsulate
 */
CollectTorque::CollectTorque(CollectTorque & source_torque)
    : active(source_torque.active),
      torque(source_torque.torque)
{
}

/**
 * CInterfaceTorque constructor for use with C torque array.
 * Note that the new CInterfaceTorque's torque *is* the torque_3vec.
 * \param[in,out] torque_3vec Torque vector to encapsulate\n Units: NM
 */
CInterfaceTorque::CInterfaceTorque(double * torque_3vec)
    : CollectTorque()
{
    active = JEOD_ALLOC_PRIM_OBJECT(bool, (true));
    torque = torque_3vec;
}

/**
 * CInterfaceTorque destructor; frees 'active' but not the torque.
 */
CInterfaceTorque::~CInterfaceTorque()
{
    JEOD_DELETE_OBJECT(active);
}

/**
 * Create a shallow copy of a Torque.
 * Note that the new CollectTorque refers to the Torque's
 * active flag and torque array.
 * @return Constructed CollectTorque
 * \param[in,out] source_torque Torque object to encapsulate
 */
CollectTorque * CollectTorque::create(Torque & source_torque)
{
    return JEOD_ALLOC_CLASS_OBJECT(CollectTorque, (source_torque));
}

/**
 * Create a shallow copy of a Torque.
 * Note that the new CollectTorque refers to the Torque's
 * active flag and torque array.
 * @return Constructed CollectTorque
 * \param[in,out] source_torque Torque object to encapsulate
 */
CollectTorque * CollectTorque::create(Torque * source_torque)
{
    return CollectTorque::create(*source_torque);
}

/**
 * Create a CollectTorque whose torque is the specified array.
 * Note that the created instance is actually a CInterfaceTorque.
 * @return Constructed CollectTorque
 * \param[in,out] torque_3vec Torque vector to encapsulate\n Units: NM
 */
CollectTorque * CollectTorque::create(double * torque_3vec)
{
    return JEOD_ALLOC_CLASS_OBJECT(CollectTorque, (torque_3vec));
}

/**
 * Create a shallow copy of a CollectTorque.
 * Note that both the source and new CollectTorques refer
 * to the same active flag and torque array.
 * @return Constructed CollectTorque
 * \param[in,out] source_torque Torque to copy
 */
CollectTorque * CollectTorque::create(CollectTorque & source_torque)
{
    return JEOD_ALLOC_CLASS_OBJECT(CollectTorque, (source_torque));
}

/**
 * Create a shallow copy of a CollectTorque.
 * Note that both the source and new CollectTorques refer
 * to the same active flag and torque array.
 * @return Constructed CollectTorque
 * \param[in,out] source_torque Torque to copy
 */
CollectTorque * CollectTorque::create(CollectTorque * source_torque)
{
    return CollectTorque::create(*source_torque);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
