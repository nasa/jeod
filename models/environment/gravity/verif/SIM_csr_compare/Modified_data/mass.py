# Set the mass porperties for this vehicle.
vehicle.mass_init.set_subject_body( vehicle.dyn_body.mass )

# Set the struct to body reference frame orientation.
vehicle.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputQuaternion

# Set the vehicle base mass.
vehicle.mass_init.properties.mass    = 400000.0
vehicle.mass_init.properties.position= [ -3.0, -1.5, 4.0]
vehicle.mass_init.properties.inertia = [[1.02e+8,-6.96e+6,-5.48e+6],
                                                  [ -6.96e+6, 0.91e+8, 5.90e+5],
                                                  [ -5.48e+6, 5.90e+5, 1.64e+8]]
dynamics.dyn_manager.add_body_action(vehicle.mass_init)
