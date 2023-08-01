/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_ut1.cc
 * Define member functions for Universal Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_ut1.cc)
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
#include "../include/time_ut1.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a Time_UT1
 */
TimeUT1::TimeUT1 (
   void)
{
   name         = "UT1";
   true_ut1     = true;
   set_epoch();
}

/**
 * Sets the epoch for UT1 time
 */
void
TimeUT1::set_epoch(
   void)
{
   tjt_at_epoch = 11544.499261238;  // J2000, Jan 1 2000:: 11:58:56.171
   return;
}

/**
 * Accesses days
 * @return days value\n Units: d
 */
double
TimeUT1::get_days (
   void)
{
   return days;
}


/**
 * Destroy a Time_UT1
 */
TimeUT1::~TimeUT1 (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
