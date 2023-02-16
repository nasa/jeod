vehicle.mass_init.set_subject_body( vehicle.dyn_body.mass )
vehicle.mass_init.properties.pt_orientation.data_source = \
                                       trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle  = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis   = [ 0.0, 1.0, 0.0]
vehicle.mass_init.properties.mass        = 80.0
vehicle.mass_init.properties.position    = [ 0.0, 0.0, 0.0]
vehicle.mass_init.properties.inertia  = [[  630.00, -277.50,  372.50],
                                         [ -277.50,  773.75,  285.00],
                                         [  372.50,  285.00,  573.75]]
dynamics.dyn_manager.add_body_action(vehicle.mass_init)
