vehicle.earth_grav_control.source_name = "Earth"
vehicle.earth_grav_control.active = True
vehicle.earth_grav_control.spherical = True

vehicle.moon_grav_control.source_name = "Moon"
vehicle.moon_grav_control.active = True
vehicle.moon_grav_control.spherical = True

vehicle.sun_grav_control.source_name = "Sun"
vehicle.sun_grav_control.active = True
vehicle.sun_grav_control.spherical = True

vehicle.dyn_body.grav_interaction.add_control(vehicle.earth_grav_control)
vehicle.dyn_body.grav_interaction.add_control(vehicle.moon_grav_control)
vehicle.dyn_body.grav_interaction.add_control(vehicle.sun_grav_control)
