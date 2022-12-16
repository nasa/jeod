vehicle.grav_control.source_name = "Earth"
vehicle.grav_control.active = True
vehicle.grav_control.spherical = False
vehicle.grav_control.degree = 4
vehicle.grav_control.order = 4

vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)
