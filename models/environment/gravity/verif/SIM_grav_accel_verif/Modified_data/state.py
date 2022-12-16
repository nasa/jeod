vehicle.trans_init.subject                  = vehicle.dyn_body.mass
vehicle.trans_init.reference_ref_frame_name = "Earth.inertial"
vehicle.trans_init.body_frame_id            = "composite_body"
vehicle.trans_init.position  = [ -4292653.41, 955168.47, 5139356.57]
vehicle.trans_init.velocity  = [ 109.649663, -7527.726490, 1484.521489]

dynamics.dyn_manager.add_body_action(vehicle.trans_init)

vehicle.lvlh_init.subject                 = vehicle.dyn_body.mass
vehicle.lvlh_init.planet_name             = "Earth"
vehicle.lvlh_init.body_frame_id           = "composite_body"
vehicle.lvlh_init.orientation.data_source = trick.Orientation.InputEulerRotation
vehicle.lvlh_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
vehicle.lvlh_init.orientation.euler_angles   = \
                                     trick.attach_units( "degree",[ 0.0, -11.6, 0.0])
vehicle.lvlh_init.ang_velocity = [ 0.0, 0.0, 0.0]

dynamics.dyn_manager.add_body_action(vehicle.lvlh_init)
