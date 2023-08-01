vehicle.mass_init.set_subject_body(vehicle.dyn_body.mass)
vehicle.mass_init.properties.mass =  500.0
vehicle.mass_init.properties.position =  [0,0,0]
vehicle.mass_init.properties.inertia[0] = [1,0,0]
vehicle.mass_init.properties.inertia[1] = [0,1,0]
vehicle.mass_init.properties.inertia[2] = [0,0,1]
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]


vehicle.mass_init.num_points = 1
vehicle.mass_init.points = trick.sim_services.alloc_type( 1 , "jeod::MassPointInit" )
vehicle.mass_init.points[0].name = "vehicle_point"
vehicle.mass_init.points[0].position  = [-1.0, 0.0, 0.0]

vehicle.mass_init.points[0].pt_orientation.data_source=trick.Orientation.InputEigenRotation
vehicle.mass_init.points[0].pt_orientation.eigen_angle = trick.attach_units( "degree",180)
vehicle.mass_init.points[0].pt_orientation.eigen_axis = [0,0,1]
