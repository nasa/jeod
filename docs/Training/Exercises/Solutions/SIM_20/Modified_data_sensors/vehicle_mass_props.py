vehicle.mass_init.set_subject_body(vehicle.dyn_body.mass)
vehicle.mass_init.properties.mass =  500.0
vehicle.mass_init.properties.position =  [0,0,0]
vehicle.mass_init.properties.inertia[0] = [1,0,0]
vehicle.mass_init.properties.inertia[1] = [0,1,0]
vehicle.mass_init.properties.inertia[2] = [0,0,1]
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]


vehicle.mass_init.allocate_points(3)

vehicle.mass_init.get_mass_point(0).name = "lidar-case"
vehicle.mass_init.get_mass_point(0).position  = [1.0, 0.0, 0.0]
vehicle.mass_init.get_mass_point(0).pt_orientation.data_source=trick.Orientation.InputEigenRotation
vehicle.mass_init.get_mass_point(0).pt_orientation.eigen_angle = 0.0
vehicle.mass_init.get_mass_point(0).pt_orientation.eigen_axis = [0,0,1]

vehicle.mass_init.get_mass_point(1).name = "sun-sensor-case"
vehicle.mass_init.get_mass_point(1).position  = [0.0, 1.0, 0.0]
vehicle.mass_init.get_mass_point(1).pt_orientation.data_source=trick.Orientation.InputEigenRotation
vehicle.mass_init.get_mass_point(1).pt_orientation.eigen_angle = 0.0
vehicle.mass_init.get_mass_point(1).pt_orientation.eigen_axis = [0,0,1]

vehicle.mass_init.get_mass_point(2).name = "imu-case"
vehicle.mass_init.get_mass_point(2).position  = [-1.0, 0.0, 0.0]
vehicle.mass_init.get_mass_point(2).pt_orientation.data_source=trick.Orientation.InputEigenRotation
vehicle.mass_init.get_mass_point(2).pt_orientation.eigen_angle = 0.0
vehicle.mass_init.get_mass_point(2).pt_orientation.eigen_axis = [0,0,1]
