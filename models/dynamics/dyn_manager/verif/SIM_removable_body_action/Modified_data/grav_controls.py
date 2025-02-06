# Set up the gravity controls for the Earth.
vehicle.earth_grav_control.source_name     = "Earth"
vehicle.earth_grav_control.active          = True
vehicle.earth_grav_control.spherical       = True
vehicle.earth_grav_control.gradient        = True

# Set up the gravity controls for the Moon.
vehicle.moon_grav_control.source_name = "Moon"
vehicle.moon_grav_control.active      = False
vehicle.moon_grav_control.spherical   = True
vehicle.moon_grav_control.gradient    = True

# Set up the gravity controls for the Sun.
vehicle.sun_grav_control.source_name = "Sun"
vehicle.sun_grav_control.active      = False
vehicle.sun_grav_control.spherical   = True
vehicle.sun_grav_control.gradient    = True

#CALL
vehicle.dyn_body.grav_interaction.add_control(vehicle.earth_grav_control)
vehicle.dyn_body.grav_interaction.add_control(vehicle.moon_grav_control)
vehicle.dyn_body.grav_interaction.add_control(vehicle.sun_grav_control)
