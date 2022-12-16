#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (Compute the wind velocity wrt inertial and given in inertial frame. This
#    function also computes the vehicle's altitude above the planet.)
#REFERENCE:
#   (((King-Hele Rotational Atmospheric Model)
#     (Zarrouati, G., Trajectoires Spatiales) (CNES, CEPADUES-Editions)
#     (Toulouse, France,) (1987, pp.87-89)))
#ASSUMPTIONS AND LIMITATIONS:
#   ((These values create a wind speed as a linear finction of altitude.))
# 
#*/

def set_Earth_uniform_wind(earth) :

  #/* Make sure that the planetary rotation rate is set consistently. */
  earth.wind_velocity.omega = earth.rnp.planet_omega

  #/* Initialize altitude and scaling factor. */
  earth.wind_velocity.set_omega_scale_table(0.0, 1.0)


  return
