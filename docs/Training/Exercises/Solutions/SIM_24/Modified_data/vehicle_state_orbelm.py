orbit_init = trick.DynBodyInitOrbit()
orbit_init.thisown = 0

orbit_init.dyn_subject		    = vehicle.dyn_body
orbit_init.reference_ref_frame_name = "Earth.inertial"
orbit_init.body_frame_id 	    = "composite_body"
orbit_init.set = trick.DynBodyInitOrbit.SmaEccIncAscnodeArgperTimeperi
orbit_init.orbit_frame_name = "inertial"
orbit_init.planet_name = "Earth"

orbit_init.semi_major_axis = trick.sim_services.attach_units("km" , 6700.0)
orbit_init.inclination     = trick.sim_services.attach_units("degree" , 52.0)
orbit_init.eccentricity    = 0.01
orbit_init.arg_periapsis   = trick.sim_services.attach_units("degree" , 60.0)
orbit_init.ascending_node  = trick.sim_services.attach_units("degree" , 25.0)
orbit_init.time_periapsis  = 0.0

# vehicle_basic already includes a standard RotInit instance
vehicle.rot_init.dyn_subject 		  =  vehicle.dyn_body
vehicle.rot_init.reference_ref_frame_name = "Earth.inertial"
vehicle.rot_init.body_frame_id            = "composite_body"
vehicle.rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
vehicle.rot_init.orientation.eigen_angle  = 0.0
vehicle.rot_init.orientation.eigen_axis   = [  0,   0,   1]
vehicle.rot_init.ang_velocity	          = [0.0, 0.0, 0.0]


dynamics.dyn_manager.add_body_action( orbit_init )
dynamics.dyn_manager.add_body_action( vehicle.rot_init )
