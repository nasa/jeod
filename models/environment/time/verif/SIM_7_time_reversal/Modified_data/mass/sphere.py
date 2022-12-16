def set_mass_sphere(veh_obj_reference) :

  # Set the mass porperties for this vehicle.
  veh_obj_reference.mass_init.subject = veh_obj_reference.body.mass

  # Set the struct to body reference frame orientation.
  veh_obj_reference.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


  veh_obj_reference.mass_init.properties.pt_orientation.eigen_angle  = 0.0
  veh_obj_reference.mass_init.properties.pt_orientation.eigen_axis    = [ 0.0, 1.0, 0.0]

  # Set the vehicle base mass.
  veh_obj_reference.mass_init.properties.mass              = 1.0
  veh_obj_reference.mass_init.properties.position        = [ 0.0, 0.0, 0.0]
  # Value used to be 2/(5*M_PI), changed to a decimal.
  veh_obj_reference.mass_init.properties.inertia[0]  = \
              [[ 0.127323954474, 0.0,            0.0],
               [ 0.0,            0.127323954474, 0.0],
               [ 0.0,            0.0,            0.127323954474]]
