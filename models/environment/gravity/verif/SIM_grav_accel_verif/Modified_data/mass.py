vehicle.mass_init.set_subject_body( vehicle.dyn_body.mass )
vehicle.mass_init.properties.pt_orientation.data_source = \
                                       trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]

vehicle.mass_init.properties.mass        = 400000.0
vehicle.mass_init.properties.position    = [ -3.0, -1.5, 4.0]
vehicle.mass_init.properties.inertia[0]  = [  1.02e+8,-6.96e+6,-5.48e+6]
vehicle.mass_init.properties.inertia[1]  = [ -6.96e+6, 0.91e+8, 5.90e+5]
vehicle.mass_init.properties.inertia[2]  = [ -5.48e+6, 5.90e+5, 1.64e+8]
dynamics.dyn_manager.add_body_action(vehicle.mass_init)
