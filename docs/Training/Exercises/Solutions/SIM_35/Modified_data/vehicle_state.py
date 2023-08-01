vehicleA.orb_init.set_subject_body(vehicleA.dyn_body)
vehicleA.orb_init.body_frame_id    = "composite_body"
vehicleA.orb_init.planet_name      = "Earth"
vehicleA.orb_init.orbit_frame_name = "pfix"
vehicleA.orb_init.reference_ref_frame_name = "Earth.pfix"
vehicleA.orb_init.set = trick.DynBodyInitOrbit.SmaEccIncAscnodeArgperTimeperi
vehicleA.orb_init.semi_major_axis  =  trick.sim_services.attach_units("km", 6700)
vehicleA.orb_init.eccentricity = 0.0
vehicleA.orb_init.inclination = 0.0

vehicleA.lvlh_rot_init.set_subject_body(vehicleA.dyn_body)
vehicleA.lvlh_rot_init.planet_name    = "Earth"
vehicleA.lvlh_rot_init.body_frame_id  = "composite_body"
vehicleA.lvlh_rot_init.orientation.data_source = trick.Orientation.InputEigenRotation
vehicleA.lvlh_rot_init.orientation.eigen_angle = 0.0
vehicleA.lvlh_rot_init.orientation.eigen_axis  = [0, 0, 1]
vehicleA.lvlh_rot_init.ang_velocity            = [0.0, 0.0, 0.0]

vehicleB.lvlh_trans_init.set_subject_body(vehicleB.dyn_body)
vehicleB.lvlh_trans_init.planet_name    = "Earth"
vehicleB.lvlh_trans_init.body_frame_id  = "composite_body"
vehicleB.lvlh_trans_init.ref_body_name  = "vehicleA"
vehicleB.lvlh_trans_init.position   = [0.0, 0.0, 50.0]
vehicleB.lvlh_trans_init.velocity   = [0.0, 0.0, 0.0]

vehicleB.lvlh_rot_init.set_subject_body(vehicleB.dyn_body)
vehicleB.lvlh_rot_init.planet_name    = "Earth"
vehicleB.lvlh_rot_init.body_frame_id  = "composite_body"
vehicleB.lvlh_rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
vehicleB.lvlh_rot_init.orientation.eigen_angle  = 0.0
vehicleB.lvlh_rot_init.orientation.eigen_axis   = [0, 0, 1]
vehicleB.lvlh_rot_init.ang_velocity  = [0.0, 0.0, 0.0]

dynamics.dyn_manager.add_body_action( vehicleA.orb_init )
dynamics.dyn_manager.add_body_action( vehicleA.lvlh_rot_init )
dynamics.dyn_manager.add_body_action( vehicleB.lvlh_trans_init )
dynamics.dyn_manager.add_body_action( vehicleB.lvlh_rot_init )
