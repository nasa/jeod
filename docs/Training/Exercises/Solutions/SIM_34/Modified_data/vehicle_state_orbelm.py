vehicle.orbit_init.dyn_subject		    = vehicle.dyn_body
vehicle.orbit_init.reference_ref_frame_name = "Earth.inertial"
vehicle.orbit_init.body_frame_id 	    = "composite_body"
vehicle.orbit_init.set = trick.DynBodyInitOrbit.SmaEccIncAscnodeArgperTimeperi
vehicle.orbit_init.orbit_frame_name = "inertial"
vehicle.orbit_init.planet_name = "Earth"

vehicle.orbit_init.semi_major_axis  = trick.sim_services.attach_units("km" , 6700.0)
vehicle.orbit_init.inclination  = trick.sim_services.attach_units("degree" , 52.0)
vehicle.orbit_init.eccentricity  = 0.01
vehicle.orbit_init.arg_periapsis  = trick.sim_services.attach_units("degree" , 60.0)
vehicle.orbit_init.ascending_node  = trick.sim_services.attach_units("degree" , 25.0)
vehicle.orbit_init.time_periapsis  = trick.sim_services.attach_units("s" , 0.0)

dynamics.dyn_manager.add_body_action( vehicle.orbit_init )
