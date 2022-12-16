moon_vehicle.earth_grav_control.source_name = "Earth"
moon_vehicle.earth_grav_control.active      = True
moon_vehicle.earth_grav_control.spherical   = True
moon_vehicle.earth_grav_control.gradient    = False

# Set up the gravity controls for the Moon.
moon_vehicle.moon_grav_control.source_name = "Moon"
moon_vehicle.moon_grav_control.active      = True
moon_vehicle.moon_grav_control.spherical   = False
moon_vehicle.moon_grav_control.degree      = 60
moon_vehicle.moon_grav_control.order       = 60  
moon_vehicle.moon_grav_control.gradient    = False

# Set up the gravity controls for the Sun.
moon_vehicle.sun_grav_control.source_name = "Sun"
moon_vehicle.sun_grav_control.active      = True
moon_vehicle.sun_grav_control.spherical   = True
moon_vehicle.sun_grav_control.gradient    = False

moon_vehicle.dyn_body.grav_interaction.add_control(moon_vehicle.earth_grav_control)
moon_vehicle.dyn_body.grav_interaction.add_control(moon_vehicle.moon_grav_control)
moon_vehicle.dyn_body.grav_interaction.add_control(moon_vehicle.sun_grav_control)
