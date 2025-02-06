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
  ((time_tt.o)
   (time.o)
   (time_standard.o)
   (utils/sim_interface/memory_interface.o)
   (utils/named_item/named_item.o))

 
******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "utils/named_item/include/named_item.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/time_tt.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a Time_TT
 */
TimeTT::TimeTT (
   void)
{
   name         = "TT";
   set_epoch();
}


/**
 * Sets the epoch for TT time
 */
void
TimeTT::set_epoch(
   void)
{
   tjt_at_epoch = 11544.5; // J2000, Jan 1, 2000:: 12:00:00.0
   return;
}



/**
 * Destroy a Time_TT
 */
TimeTT::~TimeTT (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
