/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/MET/src/MET_atmosphere_state_vars.cc
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

// Model incudes
#include "../include/MET_atmosphere_state_vars.hh"

//! Namespace jeod
namespace jeod
{

/*****************************************************************************
Constructor
*****************************************************************************/

/****************************************************************************/
METAtmosphereStateVars::METAtmosphereStateVars(Atmosphere & atmos_model_, const PlanetFixedPosition & pfix_pos_)
    : AtmosphereState(atmos_model_, pfix_pos_)
{
}

/**
 * Copy Constructor
 * \param[in] rhs The METAtmosphereStateVars to copy
 */

METAtmosphereStateVars::METAtmosphereStateVars(const METAtmosphereStateVars & rhs)
    : AtmosphereState(rhs)
{
    active = rhs.active;
    exo_temp = rhs.exo_temp;
    log10_dens = rhs.log10_dens;
    mol_weight = rhs.mol_weight;
    N2 = rhs.N2;
    Ox2 = rhs.Ox2;
    Ox = rhs.Ox;
    A = rhs.A;
    He = rhs.He;
    Hyd = rhs.Hyd;
}

/**
 * METAtmosphereStateVars operator =
 * @return The newly copied into METAtmosphereStateVars
 * \param[in] rhs The METAtmosphereStateVars to copy from
 */

METAtmosphereStateVars & METAtmosphereStateVars::operator=(const METAtmosphereStateVars & rhs)
{
    if(this == &rhs)
    {
        return *this;
    }

    active = rhs.active;
    exo_temp = rhs.exo_temp;
    log10_dens = rhs.log10_dens;
    mol_weight = rhs.mol_weight;
    N2 = rhs.N2;
    Ox2 = rhs.Ox2;
    Ox = rhs.Ox;
    A = rhs.A;
    He = rhs.He;
    Hyd = rhs.Hyd;

    AtmosphereState::operator=(rhs);
    return *this;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
