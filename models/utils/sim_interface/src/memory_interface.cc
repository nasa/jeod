/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/memory_interface.cc
 * Implement the MemoryInterface class.
 */

/*******************************************************************************

Purpose:
   ()

Library dependencies:
   ((memory_interface.o))

 

*******************************************************************************/


// System includes

// JEOD includes

// Model includes
#include "../include/memory_interface.hh"



//! Namespace jeod
namespace jeod {

/**
 * Default constructor.
 */
JeodMemoryInterface::JeodMemoryInterface (
   void)
{
   ; // Nothing to do.
}


/**
 * Destructor.
 */
JeodMemoryInterface::~JeodMemoryInterface (
   void)
{
   ; // Nothing to do.
}


/**
 * Copy constructor.
 * \param[in] src Item to be copied
 */
JeodMemoryInterface::JeodMemoryInterface (
   const JeodMemoryInterface & src JEOD_UNUSED)
{
   ; // Nothing to do.
}


/**
 * Assignment operator.
 * @return *this
 * \param[in] src Item to be copied
 */
JeodMemoryInterface &
JeodMemoryInterface::operator= (
   const JeodMemoryInterface & src JEOD_UNUSED)
{
   return *this;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
