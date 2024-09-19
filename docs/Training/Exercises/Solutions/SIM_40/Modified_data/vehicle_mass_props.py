def mass_props(VEH):
  VEH.mass_init.set_subject_body(VEH.dyn_body.mass)
  VEH.mass_init.properties.mass = 10.0
  VEH.mass_init.properties.position = [0,5.0,0]
  VEH.mass_init.properties.inertia[0] = [1,0,0]
  VEH.mass_init.properties.inertia[1] = [0,1,0]
  VEH.mass_init.properties.inertia[2] = [0,0,1]
  VEH.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
  VEH.mass_init.properties.pt_orientation.eigen_angle = trick.attach_units( "degree",90)
  VEH.mass_init.properties.pt_orientation.eigen_axis = [0,0,1]


  VEH.mass_init.allocate_points(1)

  VEH.mass_init.get_mass_point(0).name = "point"
  VEH.mass_init.get_mass_point(0).position  = [1.0, 0.0, 0.0]
  VEH.mass_init.get_mass_point(0).pt_orientation.data_source=trick.Orientation.InputEigenRotation
  VEH.mass_init.get_mass_point(0).pt_orientation.eigen_angle = 0.0
  VEH.mass_init.get_mass_point(0).pt_orientation.eigen_axis = [0,0,1]

  dynamics.dyn_manager.add_body_action( VEH.mass_init )
