# Make sure that the planetary rotation rate is set consistently.
earth.wind_velocity.omega = earth.rnp.planet_omega

#/* Initialize altitude and scaling factor. */
earth.wind_velocity.set_omega_scale_table(0.0, 1.0)
