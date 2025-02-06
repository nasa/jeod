/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_tt.cc
 * Define member functions for Terrestrial Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_tt.cc)
   (time.cc)
   (time_standard.cc)
   (utils/named_item/src/named_item.cc))


******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/time_tt.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Construct a Time_TT
 */
TimeTT::TimeTT()
{
    name = "TT";
    set_epoch();
}

/**
 * Sets the epoch for TT time
 */
void TimeTT::set_epoch()
{
    tjt_at_epoch = 11544.5; // J2000, Jan 1, 2000:: 12:00:00.0
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
