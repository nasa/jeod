# State initialization data for target vehicle.
target.trans_init.set_subject_body( target.dyn_body )
target.trans_init.action_name = "target_trans_init"
target.trans_init.reference_ref_frame_name = "Earth.inertial"
target.trans_init.body_frame_id = "composite_body"
target.trans_init.position  = [ 1244540.5300,  5655938.8500,  3425643.2200]
target.trans_init.velocity  = [ -6003.8330510, -1469.4960440,  4590.5117760]
dynamics.dyn_manager.add_body_action(target.trans_init)

target.rot_init.set_subject_body( target.dyn_body )
target.rot_init.action_name = "target_att_init"
target.rot_init.reference_ref_frame_name = "Earth.inertial"
target.rot_init.body_frame_id = "composite_body"
target.rot_init.orientation.data_source    = trick.Orientation.InputEulerRotation
target.rot_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
target.rot_init.orientation.euler_angles   = trick.attach_units( "degree",[ 77.590713, -30.604895, -46.100115])
target.rot_init.ang_velocity               = trick.attach_units( "degree/s",[ 0.0, -0.06556131568278, 0.0])
dynamics.dyn_manager.add_body_action(target.rot_init)
