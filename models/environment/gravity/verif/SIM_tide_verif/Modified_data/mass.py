# Set the mass properties for this vehicle.
sv_dyn.mass_init.subject = sv_dyn.dyn_body.mass

# Set the struct to body reference frame orientation.
sv_dyn.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation
sv_dyn.mass_init.properties.pt_orientation.eigen_angle = 0.0
sv_dyn.mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]

# Set the vehicle base mass.
sv_dyn.mass_init.properties.mass     = 400000.0
sv_dyn.mass_init.properties.position = [ -3.0, -1.5, 4.0]
sv_dyn.mass_init.properties.inertia  = [ [ 1.02e+8,-6.96e+6,-5.48e+6], \
                                         [ -6.96e+6, 0.91e+8, 5.90e+5],\
                                         [ -5.48e+6, 5.90e+5, 1.64e+8]]
dynamics.dyn_manager.add_body_action(sv_dyn.mass_init)
