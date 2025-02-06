#/* Set the mass properties for this vehicle. */
earth_vehicle.mass_init.set_subject_body( earth_vehicle.dyn_body.mass )

#/* Set the struct to body reference frame orientation. */
earth_vehicle.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation

earth_vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
earth_vehicle.mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]

#/* Set the vehicle base mass. */
earth_vehicle.mass_init.properties.mass       = 400000.0
earth_vehicle.mass_init.properties.position   = [ -3.0, -1.5, 4.0]
earth_vehicle.mass_init.properties.inertia[0] = [  1.02e+8,-6.96e+6,-5.48e+6]
earth_vehicle.mass_init.properties.inertia[1] = [ -6.96e+6, 0.91e+8, 5.90e+5]
earth_vehicle.mass_init.properties.inertia[2] = [ -5.48e+6, 5.90e+5, 1.64e+8]


dynamics.dyn_manager.add_body_action(earth_vehicle.mass_init)
