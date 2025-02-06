/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_tdb.cc
 * Define member functions Barycentric Dynamic Time.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_tdb.cc)
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
#include "../include/time_tdb.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Construct a Time_TDB
 */
TimeTDB::TimeTDB()
{
    name = "TDB";
    set_epoch();
}

/**
 * Sets the epoch for TDB time
 */
void TimeTDB::set_epoch()
{
    tjt_at_epoch = 11544.499999999159; // TDB is 7.2659E-5 s behind TT at J2000
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
