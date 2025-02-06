# Set up the gravity controls for the Earth.
earth_vehicle.earth_grav_control.source_name     = "Earth"
earth_vehicle.earth_grav_control.active          = True
earth_vehicle.earth_grav_control.spherical       = False
earth_vehicle.earth_grav_control.degree          = 36
earth_vehicle.earth_grav_control.order           = 36
earth_vehicle.earth_grav_control.gradient        = True
earth_vehicle.earth_grav_control.gradient_degree = 36
earth_vehicle.earth_grav_control.gradient_order  = 36

# Set up the gravity controls for the Moon.
earth_vehicle.moon_grav_control.source_name = "Moon"
earth_vehicle.moon_grav_control.active      = True
earth_vehicle.moon_grav_control.spherical   = True
earth_vehicle.moon_grav_control.gradient    = True

# Set up the gravity controls for the Sun.
earth_vehicle.sun_grav_control.source_name = "Sun"
earth_vehicle.sun_grav_control.active      = True
earth_vehicle.sun_grav_control.spherical   = True
earth_vehicle.sun_grav_control.gradient    = True

earth_vehicle.dyn_body.grav_interaction.add_control(earth_vehicle.earth_grav_control)
earth_vehicle.dyn_body.grav_interaction.add_control(earth_vehicle.moon_grav_control)
earth_vehicle.dyn_body.grav_interaction.add_control(earth_vehicle.sun_grav_control)
