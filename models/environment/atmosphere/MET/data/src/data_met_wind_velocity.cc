/*
PURPOSE:
    (Compute the wind velocity wrt inertial and given in inertial frame. This
     function also computes the vehicle's altitude above the planet.)
REFERENCE:
    (((King-Hele Rotational Atmospheric Model)
      (Zarrouati, G., Trajectoires Spatiales) (CNES, CEPADUES-Editions)
      (Toulouse, France,) (1987, pp.87-89))
     ((King-Hele D. and Walker D.)
      (Upper-Atmosphere Zonal Winds From Satellite Orbit Analysis)
      (Planetary and Space Science, 1988)
      (Vol. 36  N. 11  pp.1085-1093)))
ASSUMPTIONS AND LIMITATIONS:
    ((none))

*/

/* Allocate the altitude layers */

class WindVelocity;
#define JEOD_FRIEND_CLASS WindVelocity_wind_velocity_default_data

#include <cstddef> // NULL
// JEOD includes
#include "environment/atmosphere/base_atmos/include/wind_velocity.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Unsupported includes
#include "../include/met_data_wind_velocity.hh"

//! Namespace jeod
namespace jeod
{

/*****************************************************************************
initialize
Purpose:(initializes the WindVelocity model with these data.
         WindVelocity has only pointers to the data, so set those pointers to the
         data elements from this class.)
Limitations: (First method is deprecated)
*****************************************************************************/
void WindVelocity_wind_velocity_default_data::initialize(WindVelocity * WindVelocity_ptr)
{
    if(WindVelocity_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             "Invalid argument\n",
                             "Must pass in a legitimate pointer to a WindVelocity instance.\n");
        return;
    }
    initialize(*WindVelocity_ptr);
}

//****************************************************************************
void WindVelocity_wind_velocity_default_data::initialize(WindVelocity & wind_velocity)
{
    wind_velocity.omega = omega;
    wind_velocity.set_omega_scale_table(num_layers, omega_scale_alt, omega_scale_fac);
}
} // namespace jeod
