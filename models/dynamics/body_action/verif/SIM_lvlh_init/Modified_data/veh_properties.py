def set_veh_properties( vehicle, name) :
  vehicle.dyn_body.set_name( name )
  vehicle.lvlh.subject_name = name + ".composite_body"
  vehicle.dyn_body.integ_frame_name = "Earth.inertial"
  vehicle.dyn_body.translational_dynamics = True
  vehicle.dyn_body.rotational_dynamics = True

  vehicle.lvlh.planet_name     = "Earth"

  # Set the mass properties for given vehicle.
  vehicle.mass_init.set_subject_body( vehicle.dyn_body.mass )
  vehicle.mass_init.action_name = "vehicle_mass_init"

  # Total mass of this body.
  vehicle.mass_init.properties.mass = 1.0

  # Location of the CM in the structural reference frame.
  vehicle.mass_init.properties.position = [ 0.0, 0.0, 0.0]

  # Inertia matrix in specified initialization frame.
  vehicle.mass_init.properties.inertia[0]  = [   1.0,     0.0,     0.0]
  vehicle.mass_init.properties.inertia[1]  = [   0.0,     1.0,     0.0]
  vehicle.mass_init.properties.inertia[2]  = [   0.0,     0.0,     1.0]
  vehicle.mass_init.properties.pt_orientation.data_source = \
     trick.Orientation.InputEigenRotation
  vehicle.mass_init.properties.pt_orientation.eigen_angle =  0.0
  vehicle.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]

  dynamics.dyn_manager.add_body_action(vehicle.mass_init)
