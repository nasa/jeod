vehicle2.mass_init.set_subject_body(vehicle2.dyn_body.mass)
vehicle2.mass_init.properties.mass =  500.0
vehicle2.mass_init.properties.position =  [0,0,0]
vehicle2.mass_init.properties.inertia_spec = trick.MassPropertiesInit.Struct
vehicle2.mass_init.properties.inertia[0] = [1,0,0]
vehicle2.mass_init.properties.inertia[1] = [0,1,0]
vehicle2.mass_init.properties.inertia[2] = [0,0,1]
vehicle2.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle2.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle2.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]
