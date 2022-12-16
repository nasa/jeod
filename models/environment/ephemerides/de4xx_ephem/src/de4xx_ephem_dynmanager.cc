/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup De4xxEphem
 * @{
 *
 * @file models/environment/ephemerides/de4xx_ephem/src/de4xx_ephem_dynmanager.cc
 * Wall off dependencies on the dynamics manager.
 */

/*******************************************************************************

Purpose:
  ()

LIBRARY DEPENDENCY:
  ((de4xx_ephem_dynmanager.o)
   (de4xx_ephem.o))


*******************************************************************************/


// System includes

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "environment/time/include/time_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/de4xx_ephem.hh"


//! Namespace jeod
namespace jeod {

/**
 * Initialize the De4xxEphemeris model.
 * \param[in] time_manager Time manager
 * \param[in,out] dyn_manager Dynamics manager
 * \param[in] time_type time type
 */
void
De4xxEphemeris::initialize_model (
   const TimeManager & time_manager,
   DynManager & dyn_manager,
   std::string  time_type)
{
   initialize_model (
      time_manager, static_cast<EphemeridesManager &>(dyn_manager), time_type);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
