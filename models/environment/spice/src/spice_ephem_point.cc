/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Spice
 * @{
 *
 * @file models/environment/spice/src/spice_ephem_point.cc
 * Define the methods for the SPICE-specific ephemeris point class.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependencies:
  ((environment/ephemerides/ephem_item/src/ephem_point.cc))


*******************************************************************************/


// System includes
#include <string>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/spice_ephem_point.hh"


//! Namespace jeod
namespace jeod {

/**
 * SpiceEphemPoint default constructor.
 */
SpiceEphemPoint::SpiceEphemPoint (
   void)
:
   status(Active),
   spice_id(32767),
   parent_id(32767)
{
   JEOD_REGISTER_CLASS (SpiceEphemPoint);
}


/**
 * SpiceEphemPoint destructor.
 */
SpiceEphemPoint::~SpiceEphemPoint (
   void)
{
   ; // Nothing currently
}


/**
 * Set the active status.
 * \param[in] new_status New status value
 */
void
SpiceEphemPoint::set_status (
   SpiceEphemPoint::Status new_status)
{
   status = new_status;
   return;
}


/**
 * Return current status.
 * @return Current object status
 */
SpiceEphemPoint::Status
SpiceEphemPoint::get_status (
   void)
const
{
   return status;
}


/**
 * Set ID of associated SPICE kernel object.
 * \param[in] new_id New SPICE ID
 */
void
SpiceEphemPoint::set_spice_id (
   int new_id)
{
   spice_id = new_id;
   return;
}


/**
 * Return ID of associated SPICE kernel object.
 * @return Name
 */
int
SpiceEphemPoint::get_spice_id (
   void)
const
{
   return spice_id;
}


/**
 * Set ID of associated parent SPICE kernel object.
 * \param[in] new_id New SPICE ID
 */
void
SpiceEphemPoint::set_parent_id (
   int new_id)
{
   parent_id = new_id;
   return;
}


/**
 * Return ID of associated parent SPICE kernel object.
 * @return Name
 */
int
SpiceEphemPoint::get_parent_id (
   void)
const
{
   return parent_id;
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
