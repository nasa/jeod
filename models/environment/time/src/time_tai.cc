/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_tai.cc
 * Define member functions for International Atomic Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_tai.cc)
   (time.cc)
   (time_standard.cc)
   (utils/sim_interface/src/memory_interface.cc)
   (utils/named_item/src/named_item.cc))

 
******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_tai.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a Time_TAI
 */
TimeTAI::TimeTAI (
   void)
{
   name         = "TAI";
   set_epoch();
}


/**
 * Sets the epoch for TAI time
 */
void
TimeTAI::set_epoch(
   void)
{
   tjt_at_epoch = 11544.499627500; // J2000 Jan 1, 2000::11:59:27.816
   return;
}



/**
 * Destroy a Time_TAI
 */
TimeTAI::~TimeTAI (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
