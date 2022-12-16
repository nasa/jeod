def set_mass( name) :
  #defaults units are SI unless otherwise stated.
  vehicle.mass_init.subject = vehicle.dyn_body.mass
  vehicle.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation
  vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
  vehicle.mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]
  vehicle.mass_init.properties.position    = [ 0.0, 0.0, 0.0]
  vehicle.mass_init.properties.inertia[0]  = [ 1.0, 0.0, 0.0]
  vehicle.mass_init.properties.inertia[1]  = [ 0.0, 1.0, 0.0]
  vehicle.mass_init.properties.inertia[2]  = [ 0.0, 0.0, 1.0]

  if name == "clementine" :
    vehicle.mass_init.properties.mass = 424.0

  elif name == "rosetta" :
    vehicle.mass_init.properties.mass        = 3000

  return
