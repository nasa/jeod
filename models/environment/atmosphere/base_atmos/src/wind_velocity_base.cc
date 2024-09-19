/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/base_atmos/src/wind_velocity_base.cc
 * General base class for wind velocity models
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
   ()
Library dependencies:
   ((wind_velocity_base.cc)
    (atmosphere_messages.cc)
    (utils/message/src/message_handler.cc))

*******************************************************************************/

#include "../include/wind_velocity_base.hh"
#include "../include/atmosphere_messages.hh"
#include "utils/message/include/message_handler.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Virtual function to define the interface for inheriting functions
 * \param[in] position The position of the vehicle, however the specific implementation defines it
 * \param[in] altitude The altitude of the vehicle, however the specific implementation defines it
 * \param[out] wind_inertial The wind applied to the craft, in the inertial frame
 */

void WindVelocityBase::update_wind(double[3], double, double[3])
{
    // eventually this should be a pure virtual

    MessageHandler::warn(__FILE__,
                         __LINE__,
                         AtmosphereMessages::framework_warning,
                         "WindVelocityBase::update_wind is a virtual function with "
                         "no actual implementation. Only inheriting classes will have "
                         "full implementation. This function did nothing.\n");
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
