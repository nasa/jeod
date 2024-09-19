def mass_props(VEH):
  VEH.mass_init.set_subject_body( VEH.body )
  VEH.mass_init.properties.mass       = 10.0
  VEH.mass_init.properties.position   = [0,0,0]
  VEH.mass_init.properties.inertia[0] = [1,0,0]
  VEH.mass_init.properties.inertia[1] = [0,1,0]
  VEH.mass_init.properties.inertia[2] = [0,0,1]
  VEH.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation

  # Create mass points.
  VEH.mass_init.allocate_points(2)

  # Vehicle-Vehicle attach point
  VEH.mass_init.get_mass_point(0).name      = "Vehicle-Vehicle Attach Point"
  VEH.mass_init.get_mass_point(0).position  = [0.0, 0.0, 0.0]
  VEH.mass_init.get_mass_point(0).pt_orientation.data_source = trick.Orientation.InputEigenRotation

  # Fuel Tank attach point
  VEH.mass_init.get_mass_point(1).name      = "Vehicle-Tank Attach Point"
  VEH.mass_init.get_mass_point(1).position  = [ 0.0, 0.0, 0.0]
  VEH.mass_init.get_mass_point(1).pt_orientation.data_source = trick.Orientation.InputEigenRotation

  #Set up the tank

  mass_init = trick.MassBodyInit()
  mass_init.set_subject_body( VEH.tank )
  mass_init.properties.mass       = 10.0
  mass_init.properties.position   = [0,0,0]
  mass_init.properties.inertia[0] = [1,0,0]
  mass_init.properties.inertia[1] = [0,1,0]
  mass_init.properties.inertia[2] = [0,0,1]
  mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation

  # Create mass points.
  mass_init.allocate_points(1)

  # Vehicle B attach point
  mass_init.get_mass_point(0).name      = "Tank-Vehicle Attach Point"
  mass_init.get_mass_point(0).position  = [ 0.0, 0.0, 0.0]
  mass_init.get_mass_point(0).pt_orientation.data_source = trick.Orientation.InputEigenRotation


  # attach tank to vehicle
  mass_attach         = trick.BodyAttachAligned()
  mass_attach.thisown = 0
  mass_attach.set_subject_body( VEH.tank )
  mass_attach.set_parent_body( VEH.body )
  mass_attach.subject_point_name = "Tank-Vehicle Attach Point"
  mass_attach.parent_point_name  = "Vehicle-Tank Attach Point"


  dynamics.dyn_manager.add_body_action( VEH.mass_init )
  dynamics.dyn_manager.add_body_action( mass_init )
  dynamics.dyn_manager.add_body_action( mass_attach)
  return mass_init #Returning this to get it into the global python context so the memory is managed correctly
