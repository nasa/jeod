#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (Earth parameters used for the DIS project.)
#
#REFERENCE:
#   (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Make sure that these match the parameters in the gravity data.))
#
# 
#*/

def set_Earth_params(earth) :

  #/* Set the Earth's rotation rate. */
  earth.rnp.planet_omega  = trick.attach_units( "rad/s",7.292115146706388e-5)

  #/* Set the Earth's mass parameters. */
  earth.gravity_source.mu  = trick.attach_units( "m3/s2",3.98600436e14)

  #/* Set the Earth's ellipsoid parameters. */
  earth.planet.flat_inv           = 298.3
  earth.gravity_source.radius  = trick.attach_units( "km",6378.137)
