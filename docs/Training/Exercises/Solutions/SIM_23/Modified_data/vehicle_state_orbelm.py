state.orbit_init.dyn_subject		  = vehicle.dyn_body
state.orbit_init.reference_ref_frame_name = "Earth.inertial"
state.orbit_init.body_frame_id 	          = "composite_body"
state.orbit_init.set = trick.DynBodyInitOrbit.SmaEccIncAscnodeArgperTimeperi
state.orbit_init.orbit_frame_name = "inertial"
state.orbit_init.planet_name      = "Earth"

state.orbit_init.semi_major_axis  = trick.sim_services.attach_units("km", 6700.0)
state.orbit_init.inclination      = trick.sim_services.attach_units("degree", 52.0)
state.orbit_init.eccentricity     = 0.01
state.orbit_init.arg_periapsis    = trick.sim_services.attach_units("degree", 60.0)
state.orbit_init.ascending_node   = trick.sim_services.attach_units("degree", 25.0)
state.orbit_init.time_periapsis   = 0.0

dynamics.dyn_manager.add_body_action( state.orbit_init )
