def mass_props(VEH):
  VEH.mass_init.subject = vehicle.dyn_body.mass
  VEH.mass_init.properties.mass =  10.0 
  VEH.mass_init.properties.position =  [0,0,0] 
  VEH.mass_init.properties.inertia[0] = [1,0,0] 
  VEH.mass_init.properties.inertia[1] = [0,1,0] 
  VEH.mass_init.properties.inertia[2] = [0,0,1]
  VEH.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
  
  dynamics.dyn_manager.add_body_action( VEH.mass_init )
  return
