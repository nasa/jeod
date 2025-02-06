/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/MET/src/MET_atmosphere.cc
 * Implementation of MET atmosphere model
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   (Provides the specific implementation of the atmosphere state for a
   MET atmosphere.)

REFERENCE:
   (((Jacchia, L.G.) (New Static Models of the Thermosphere and
       Exosphere with Empirical Temperature Profiles) (Smithsonian
       Astrophysical Observatory Special Report No. 313) (--) (1970) (--))
     ((Jacchia, L.G.) (Revised Static Models of the Termosphere
       and Exosphere with Emperical Temperature Profiles)
       (Smithsonian Astrophysical Observatory Report No. 332) (--)
       (1971) (--)))

ASSUMPTIONS AND LIMITATIONS:
   ((Too many to enumerate here.))

LIBRARY DEPENDENCY:
  ((environment/atmosphere/base_atmos/src/atmosphere_messages.cc)
   (utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <cstddef> // provides NULL

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model incudes
#include "../include/MET_atmosphere_state.hh"
#include "environment/atmosphere/base_atmos/include/atmosphere_messages.hh"

//! Namespace jeod
namespace jeod
{
/***************************************************************************/
METAtmosphereState::METAtmosphereState(METAtmosphere & met_atmos_, const PlanetFixedPosition & pfix_pos_)
    : METAtmosphereStateVars(met_atmos_, pfix_pos_),
      met_atmos(&met_atmos_)
{
}

/**
 * Updates the METAtmosphereState from the METAtmosphere pointed to
 * by atmos_model_. This is a specific function for the case of
 * an METAtmosphere state updating an METAtmosphere
 * \param[in] atmos_model_ METAtmosphere Model.
 * \param[in] pfix_pos_ Current vehicle position.
 */

void METAtmosphereState::update_state(METAtmosphere * atmos_model_, const PlanetFixedPosition * pfix_pos_)
{
    if(active && (atmos_model_ != nullptr))
    {
        atmos_model_->update_atmosphere(pfix_pos_, this);
    }
}

/**
 * Updates the METAtmosphereState from the METAtmosphere pointed to
 * by class member atmos_model using class member pointer pfix_pos.
 * This is a specific function for the case of an METAtmosphere state
 * updating an METAtmosphere when constructed with the pointers set.
 */

void METAtmosphereState::update_state()
{
    if(active && (met_atmos != nullptr))
    {
        met_atmos->update_atmosphere(pfix_pos, this);
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
