state.orb_init.set_subject_body(vehicle.dyn_body)
state.orb_init.reference_ref_frame_name = "Earth.inertial"
state.orb_init.body_frame_id            = "composite_body"
state.orb_init.set = trick.DynBodyInitOrbit.SmaEccIncAscnodeArgperTimeperi
state.orb_init.orbit_frame_name = "inertial"
state.orb_init.planet_name      = "Earth"

state.orb_init.semi_major_axis  = trick.sim_services.attach_units("km", 6700.0)
state.orb_init.inclination      = 0.0
state.orb_init.eccentricity     = 0.0
state.orb_init.arg_periapsis    = 0.0
state.orb_init.ascending_node   = 0.0
state.orb_init.time_periapsis   = 0.0


vehicle.rot_init.set_subject_body(vehicle.dyn_body)
vehicle.rot_init.reference_ref_frame_name = "Earth.inertial"
vehicle.rot_init.body_frame_id            = "composite_body"
vehicle.rot_init.orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.rot_init.orientation.eigen_angle = 0.0
vehicle.rot_init.orientation.eigen_axis  = [0 ,0 ,1]
vehicle.rot_init.ang_velocity		 = [0.0, 0.0, 0.0]


dynamics.dyn_manager.add_body_action( state.orb_init )
dynamics.dyn_manager.add_body_action( vehicle.rot_init )
