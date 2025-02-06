vehicle.grav_control.source_name = "Earth"
vehicle.grav_control.active = True
vehicle.grav_control.spherical = True
vehicle.grav_control.gradient = True

vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)
