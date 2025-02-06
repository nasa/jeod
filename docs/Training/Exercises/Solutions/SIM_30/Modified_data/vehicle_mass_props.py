vehicle.mass_init.subject = vehicle.dyn_body.mass
vehicle.mass_init.properties.mass =  10.0
vehicle.mass_init.properties.position =  [0,0,0]
vehicle.mass_init.properties.inertia[0] = [1,0,0]
vehicle.mass_init.properties.inertia[1] = [0,1,0]
vehicle.mass_init.properties.inertia[2] = [0,0,1]
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation


mass_init = trick.MassBodyInit()
mass_init.thisown = 0
mass_init.subject = vehicle.array
mass_init.properties.mass =  10.0
mass_init.properties.position =  [0,0,0]
mass_init.properties.inertia[0] =  [1,0,0]
mass_init.properties.inertia[1] = [0,1,0]
mass_init.properties.inertia[2] = [0,0,1]
mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation

dynamics.dyn_manager.add_body_action( vehicle.mass_init )
dynamics.dyn_manager.add_body_action( mass_init )
