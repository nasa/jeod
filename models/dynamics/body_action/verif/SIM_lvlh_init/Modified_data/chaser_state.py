# State translational initialization data for the chaser.
chaser.lvlh_init.set_subject_body( chaser.dyn_body )
chaser.lvlh_init.action_name = "chaser_lvlh_init"
chaser.lvlh_init.planet_name = "Earth"
chaser.lvlh_init.body_frame_id = "composite_body"
chaser.lvlh_init.set_lvlh_frame_object (target.lvlh)
chaser.lvlh_init.position  = [ -100.0, 25.0, 7.5]
chaser.lvlh_init.velocity  = [ 0.0, 0.0, 1.0]

# State attitude initialization data for the chaser.
chaser.lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
chaser.lvlh_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
chaser.lvlh_init.orientation.euler_angles   = [ 0.0, 0.0, 0.0]
chaser.lvlh_init.ang_velocity               = trick.attach_units( "degree/s",[ 0.0, 0.0, 4.5])

dynamics.dyn_manager.add_body_action(chaser.lvlh_init)
