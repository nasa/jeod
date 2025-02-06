/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/base_atmos/src/atmosphere_state.cc
 * Implementation of the base atmosphere-state model
 */

/*****************************************************************************
PURPOSE:
   (Basic functionality of the generic atmosphere-state)


*****************************************************************************/

// System includes
#include <cstddef>

// JEOD includes

#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/atmosphere_state.hh"
#include "../include/wind_velocity.hh"


//! Namespace jeod
namespace jeod {

/*****************************************************************************
Constructors
*****************************************************************************/
AtmosphereState::AtmosphereState ()
   :
   active(true),
   temperature(0.0),
   density(0.0),
   pressure(0.0),
   atmos(NULL),
   pfix_pos(NULL)
{
   Vector3::initialize (wind);
}
/****************************************************************************/
AtmosphereState::AtmosphereState (
   Atmosphere                & atmos_,
   const PlanetFixedPosition & pfix_pos_)
   :
   active(true),
   temperature(0.0),
   density(0.0),
   pressure(0.0),
   atmos(    &atmos_),
   pfix_pos( &pfix_pos_)
{
   Vector3::initialize (wind);
}

/*****************************************************************************
destructor
*****************************************************************************/
AtmosphereState::~AtmosphereState ()
{ }



/**
 * Copy Constructor
 * \param[in] rhs The AtmosphereState to copy from
 */

AtmosphereState::AtmosphereState (
   const AtmosphereState& rhs)
{

   temperature = rhs.temperature;
   density     = rhs.density;
   pressure    = rhs.pressure;

   for (unsigned int ii = 0; ii < 3; ++ii) {
      wind[ii] = rhs.wind[ii];
   }
   atmos       = rhs.atmos;
   pfix_pos    = rhs.pfix_pos;
}

/**
 * AtmosphereState Operator =
 * @return The newly copied AtmosphereState
 * \param[in] rhs The AtmosphereState to copy
 */


AtmosphereState&
AtmosphereState::operator = (
   const AtmosphereState& rhs)
{

   if (this == &rhs) {
      return *this;
   }

   temperature = rhs.temperature;
   density     = rhs.density;
   pressure    = rhs.pressure;

   return *this;
}

/**
 * Updates the invoking atmosphere state, using the atmosphere
 * model pointed to by atmos_model, and calculated at the planet
 * fixed position pointed to by pfix_pos. Note that any type
 * inheriting from Atmosphere can be sent in for atmos_model.
 * \param[in] atmos_model_ Atmosphere model.
 * \param[in] pfix_pos_ Planetary fixed position.
 */

void
AtmosphereState::update_state (
   Atmosphere          * atmos_model_,
   PlanetFixedPosition * pfix_pos_)
{
   // Only call this update routine if model is set and active.
   if (active && (atmos_model_ != NULL)) {
      atmos_model_->update_atmosphere (pfix_pos_, this);
   }
}

/**
 * Updates the invoking atmosphere state, using the atmosphere
 * model pointed to by atmos, and calculated at the planet
 * fixed position pointed to by pfix_pos. Note that any type
 * inheriting from Atmosphere can used as the Atmosphere pointer
 * but only the values associated with AtmosphereState will be
 * copied back out.
 */

void
AtmosphereState::update_state ()
{
   // Only call this update routine if model is set and active.
   if (active && (atmos != NULL)) {
      atmos->update_atmosphere (pfix_pos, this);
   }
}


/**
 * Updates the wind portion of the invoking atmosphere state,
 * using the wind model pointed to by wind_vel, calculated at the
 * inertial position given by inrtl_pos and the altitude given
 * \param[in] wind_vel Wind velocity model.
 * \param[in] inrtl_pos Current inertial position.\n Units: M
 * \param[in] altitude Geodetic (elliptic) altitude.\n Units: M
 */

void
AtmosphereState::update_wind (
   WindVelocity * wind_vel,
   double         inrtl_pos[3],
   double         altitude)
{

   // Only call this update routine if model is set and active.
   if (active && (wind_vel != NULL)) {
      wind_vel->update_wind (inrtl_pos, altitude, wind);
   }
   return;

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
