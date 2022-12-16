def set_Earth_params(earth) :

  # Set the RNP name.
  earth.planet.set_name("Earth")
  earth.rnp.set_name("Earth")

  # Set the Earth's rotation rate.
  earth.rnp.planet_omega  = 7.292115146706388e-5

  # Set the Earth's mass parameters.
  earth.gravity_source.mu  = 3.98600436e14

  # Set the Earth's ellipsoid parameters.
  earth.planet.flat_inv           = 298.3
  earth.gravity_source.radius  = trick.attach_units( "km",6378.137)
