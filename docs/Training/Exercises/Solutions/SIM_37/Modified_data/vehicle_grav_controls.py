vehicleA.grav_control.source_name = "Earth"
vehicleA.grav_control.active = True
vehicleA.grav_control.spherical = True

vehicleB.grav_control.source_name = "Earth"
vehicleB.grav_control.active = True
vehicleB.grav_control.spherical = True

vehicleA.dyn_body.grav_interaction.add_control(vehicleA.grav_control)
vehicleB.dyn_body.grav_interaction.add_control(vehicleB.grav_control)
