moon_vehicle.trans_init.set_subject_body(moon_vehicle.dyn_body)
moon_vehicle.trans_init.reference_ref_frame_name = "Moon.inertial"
moon_vehicle.trans_init.body_frame_id     = "composite_body"
moon_vehicle.trans_init.position  = trick.attach_units( "km",[  1296.944012, -1060.824450, 2522.289146])
moon_vehicle.trans_init.velocity  = trick.attach_units( "km/s",[ -.930578, -.439312, .862075])

#//
#// Set the rotational position.
#//
moon_vehicle.lvlh_init.set_subject_body(moon_vehicle.dyn_body)
moon_vehicle.lvlh_init.planet_name               = "Earth"
moon_vehicle.lvlh_init.body_frame_id             = "composite_body"
moon_vehicle.lvlh_init.orientation.data_source   = trick.Orientation.InputEulerRotation
moon_vehicle.lvlh_init.orientation.euler_sequence= trick.Orientation.Yaw_Pitch_Roll
moon_vehicle.lvlh_init.orientation.euler_angles  = [ 0.0, 0.0, 0.0]
moon_vehicle.lvlh_init.ang_velocity              = [ 0.0, 0.0, 0.0]


dynamics.dyn_manager.add_body_action(moon_vehicle.trans_init)
dynamics.dyn_manager.add_body_action(moon_vehicle.lvlh_init)
