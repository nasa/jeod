vehicle.mass_init.set_subject_body(vehicle.dyn_body.mass)
vehicle.mass_init.properties.mass =  500.0
vehicle.mass_init.properties.position =  [0,0,0]
vehicle.mass_init.properties.inertia[0] = [1,0,0]
vehicle.mass_init.properties.inertia[1] = [0,1,0]
vehicle.mass_init.properties.inertia[2] = [0,0,1]
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]


vehicle.mass_init.allocate_points(1)
vehicle.mass_init.get_mass_point(0).name = "exercise 18 vehicle point"
vehicle.mass_init.get_mass_point(0).position  = [-1.0, 0.0, 0.0]

# The following 3 blocks are equivalent solutions
vehicle.mass_init.get_mass_point(0).pt_orientation.data_source=trick.Orientation.InputEigenRotation
vehicle.mass_init.get_mass_point(0).pt_orientation.eigen_angle = trick.attach_units( "degree",180)
vehicle.mass_init.get_mass_point(0).pt_orientation.eigen_axis = [0,0,1]
##
#vehicle.mass_init.get_mass_point(0).pt_orientation.data_source=trick.Orientation.InputMatrix
#vehicle.mass_init.get_mass_point(0).pt_orientation.trans[0]  = [ -1.0,  0.0, 0.0]
#vehicle.mass_init.get_mass_point(0).pt_orientation.trans[1]  = [  0.0, -1.0, 0.0]
#vehicle.mass_init.get_mass_point(0).pt_orientation.trans[2]  = [  0.0,  0.0, 1.0]
##
#vehicle.mass_init.get_mass_point(0).pt_orientation.data_source=trick.Orientation.InputEulerRotation
#vehicle.mass_init.get_mass_point(0).pt_orientation.euler_sequence = trick.Orientation.YawPitchRoll
#vehicle.mass_init.get_mass_point(0).pt_orientation.euler_angles = trick.attach_units( "degree",[180.0, 0.0, 0.0])
##
