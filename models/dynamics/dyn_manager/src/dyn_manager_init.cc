/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/dyn_manager_init.cc
 * Define member functions for the DynManagerInit class.
 */

/*****************************************************************************
Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((N/A))

Library dependencies:
  ((dyn_manager_init.o))


******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/dyn_manager_init.hh"



//! Namespace jeod
namespace jeod {

/**
 * DynManagerInit default constructor.
 */
DynManagerInit::DynManagerInit (
   void)
:
   mode(EphemerisMode_Ephemerides),
   central_point_name(NULL),
   integ_group_constructor(NULL),
   integ_constructor(NULL),
   jeod_integ_opt(er7_utils::Integration::Unspecified),
   sim_integ_opt(-1)
{
   // Empty
}


/**
 * DynManagerInit destructor.
 */
DynManagerInit::~DynManagerInit (
   void)
{
// FIXME: Allocate central point name or delete code.
// For now, the central point name is not allocated by JEOD, so commented out.
//   if (central_point_name != NULL) {
//      JEOD_DELETE_ARRAY (central_point_name);
//   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
