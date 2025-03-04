/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup PlanetFixed
 * @{
 * @addtogroup PlanetFixedPosn
 * @{
 *
 * @file models/utils/planet_fixed/planet_fixed_posn/src/alt_lat_long_state.cc
 * AltLatLongState class methods
 */

/********************************* TRICK HEADER ********************************
PURPOSE:
   ()

LIBRARY DEPENDENCY:
   ((alt_lat_long_state.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/alt_lat_long_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Allow user to set member data.
 * \param[in] alt Altitude\n Units: M
 * \param[in] lat Longitude\n Units: r
 * \param[in] lon Latitude\n Units: r
 */
void AltLatLongState::set_data(double alt, double lat, double lon)
{
    // Set class data to user inputs
    altitude = alt;
    latitude = lat;
    longitude = lon;
}

/**
 * Allow user to get member data.
 * \param[out] alt_out Altitude\n Units: M
 * \param[out] lat_out Longitude\n Units: r
 * \param[out] lon_out Latitude\n Units: r
 */
void AltLatLongState::get_data(double & alt_out, double & lat_out, double & lon_out)
{
    // Get class data from user inputs
    alt_out = altitude;
    lat_out = latitude;
    lon_out = longitude;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
