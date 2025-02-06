moon_vehicle.mass_init.subject = moon_vehicle.dyn_body.mass

#/* Set the struct to body reference frame orientation. */
moon_vehicle.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation
moon_vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
moon_vehicle.mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]

#/* Set the vehicle base mass. */
moon_vehicle.mass_init.properties.mass        = 424.0
moon_vehicle.mass_init.properties.position    = [ 0.0, 0.0, 0.0]
moon_vehicle.mass_init.properties.inertia[0]  = [ 1.0, 0.0, 0.0]
moon_vehicle.mass_init.properties.inertia[1]  = [ 0.0, 1.0 , 0.0]
moon_vehicle.mass_init.properties.inertia[2]  = [ 0.0, 0.0 , 1.0]


dynamics.dyn_manager.add_body_action(moon_vehicle.mass_init)
