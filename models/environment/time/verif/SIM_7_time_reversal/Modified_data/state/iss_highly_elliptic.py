def set_state_iss_highly_elliptic(veh_obj_reference) :

  # Set the translational position.
  veh_obj_reference.trans_init.set_subject_body( veh_obj_reference.body.mass )
  veh_obj_reference.trans_init.reference_ref_frame_name = "Earth.inertial"
  veh_obj_reference.trans_init.body_frame_id     = "composite_body"
  veh_obj_reference.trans_init.position  = [ -4315967.74, 960356.20, 5167269.53]
  veh_obj_reference.trans_init.velocity  = [ 129.091037, -7491.513855, 1452.515654]

  # Set the rotational position.
  veh_obj_reference.lvlh_init.set_subject_body( veh_obj_reference.body )
  veh_obj_reference.lvlh_init.planet_name                     = "Earth"
  veh_obj_reference.lvlh_init.body_frame_id                   = "composite_body"
  veh_obj_reference.lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
  veh_obj_reference.lvlh_init.orientation.euler_sequence      = trick.Orientation.Yaw_Pitch_Roll
  veh_obj_reference.lvlh_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, -11.6, 0.0])
  veh_obj_reference.lvlh_init.ang_velocity            = [ 0.0, 0.0, 0.0]
