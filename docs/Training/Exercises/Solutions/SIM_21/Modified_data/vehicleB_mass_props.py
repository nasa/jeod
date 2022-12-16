vehicleB.mass_init.subject = vehicleB.dyn_body.mass
vehicleB.mass_init.properties.mass     = 9.0
vehicleB.mass_init.properties.position = [1.0, 0.0, 0.0]

vehicleB.mass_init.properties.inertia[0] = [6.0, 0.0, 0.0]
vehicleB.mass_init.properties.inertia[1] = [0.0, 6.0, 0.0]
vehicleB.mass_init.properties.inertia[2] = [0.0, 0.0, 6.0]

vehicleB.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicleB.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicleB.mass_init.properties.pt_orientation.eigen_axis  = [1.0, 0.0, 0.0]

# Create mass point.
vehicleB.mass_init.num_points = 1
vehicleB.mass_init.points = trick.sim_services.alloc_type(vehicleB.mass_init.num_points, "jeod::MassPointInit")
vehicleB.mass_init.points[0].name             = "Vehicle B Mass Point 1"
vehicleB.mass_init.points[0].pt_frame_spec    = trick.MassPointInit.StructToPoint
vehicleB.mass_init.points[0].position         = [0.0, 0.0, 0.0]
vehicleB.mass_init.points[0].pt_orientation.data_source  = trick.Orientation.InputEigenRotation
vehicleB.mass_init.points[0].pt_orientation.eigen_angle  = trick.attach_units( "degree",180.0)
vehicleB.mass_init.points[0].pt_orientation.eigen_axis   = [0.0, 0.0, 1.0]
