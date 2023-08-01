/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_utc.cc
 * Define member functions for Coordinated Universal Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_utc.cc)
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
#include "../include/time_utc.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a Time_UTC
 */
TimeUTC::TimeUTC (
   void)
{
   name         = "UTC";
   true_utc     = true;
   set_epoch();
}


/**
 * Sets the epoch for UTC time
 */
void
TimeUTC::set_epoch(
   void)
{
   tjt_at_epoch = 11544.49925712963; // J2000:  Jan 1,2000 11:58:55.816
   return;
}


/**
 * Destroy a Time_UTC
 */
TimeUTC::~TimeUTC (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
