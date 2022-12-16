earth_vehicle.trans_init.dyn_subject = earth_vehicle.dyn_body
earth_vehicle.trans_init.reference_ref_frame_name = "Earth.inertial"
earth_vehicle.trans_init.body_frame_id     = "composite_body"
earth_vehicle.trans_init.position  = [ -4292653.41, 955168.47, 5139356.57]
earth_vehicle.trans_init.velocity  = [ 109.649663, -7527.726490, 1484.521489]

#//
#// Set the rotational position.
#//
earth_vehicle.lvlh_init.dyn_subject              = earth_vehicle.dyn_body
earth_vehicle.lvlh_init.planet_name              = "Earth"
earth_vehicle.lvlh_init.body_frame_id            = "composite_body"
earth_vehicle.lvlh_init.orientation.data_source  = trick.Orientation.InputEulerRotation
earth_vehicle.lvlh_init.orientation.euler_sequence= trick.Orientation.Yaw_Pitch_Roll
earth_vehicle.lvlh_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, -11.6, 0.0])
earth_vehicle.lvlh_init.ang_velocity              = [ 0.0, 0.0, 0.0]


dynamics.dyn_manager.add_body_action(earth_vehicle.trans_init)
dynamics.dyn_manager.add_body_action(earth_vehicle.lvlh_init)
