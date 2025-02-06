vehicle.grav_control.source_name = "Earth"
vehicle.grav_control.active = True
vehicle.grav_control.spherical = True

sun_grav_control = trick.SphericalHarmonicsGravityControls()
sun_grav_control.thisown = 0
sun_grav_control.source_name = "Sun"
sun_grav_control.active = True
sun_grav_control.spherical = True

vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)
vehicle.dyn_body.grav_interaction.add_control(sun_grav_control)
