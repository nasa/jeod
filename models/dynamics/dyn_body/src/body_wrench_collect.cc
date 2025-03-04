/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/body_wrench_collect.cc
 * Define BodyWrenchCollect member functions.
 */

/*
Purpose: ()
*/

#include "../include/body_wrench_collect.hh"

#include "utils/memory/include/jeod_alloc.hh"

//! Namespace jeod
namespace jeod
{

BodyWrenchCollect::BodyWrenchCollect()
    : collect_wrench()
{
    JEOD_REGISTER_CLASS(BodyWrenchCollect);
    JEOD_REGISTER_CHECKPOINTABLE(this, collect_wrench);
}

BodyWrenchCollect::~BodyWrenchCollect()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, collect_wrench);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
