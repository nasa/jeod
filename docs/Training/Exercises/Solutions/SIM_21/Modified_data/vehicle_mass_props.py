vehicle.mass_init.set_subject_body(vehicle.dyn_body.mass)
vehicle.mass_init.properties.mass = 3.0
vehicle.mass_init.properties.position = [3.0,0,0]

vehicle.mass_init.properties.inertia[0] = [2.0, 0.0, 0.0]
vehicle.mass_init.properties.inertia[1] = [0.0, 2.0, 0.0]
vehicle.mass_init.properties.inertia[2] = [0.0, 0.0, 2.0]
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [1.0, 0.0, 0.0]

# Create mass point.
vehicle.mass_init.allocate_points(1)

vehicle.mass_init.get_mass_point(0).name          = "Vehicle A Mass Point 1"
vehicle.mass_init.get_mass_point(0).pt_frame_spec = trick.MassPointInit.StructToPoint
vehicle.mass_init.get_mass_point(0).position      = [4.0, 0.0, 0.0]
vehicle.mass_init.get_mass_point(0).pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.get_mass_point(0).pt_orientation.eigen_angle = 0.0
vehicle.mass_init.get_mass_point(0).pt_orientation.eigen_axis  = [1.0, 0.0, 0.0]
