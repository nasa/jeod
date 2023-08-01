vehicle.mass_init.set_subject_body(vehicle.dyn_body.mass)
vehicle.mass_init.properties.mass       =  500.0
vehicle.mass_init.properties.position   = [0,0,0]
vehicle.mass_init.properties.inertia[0] = [ 0.1,-0.2,-0.2]
vehicle.mass_init.properties.inertia[1] = [-0.2, 0.7, 0.0]
vehicle.mass_init.properties.inertia[2] = [-0.2, 0.0, 0.7]
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]
