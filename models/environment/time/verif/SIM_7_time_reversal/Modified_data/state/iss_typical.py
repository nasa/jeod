def set_state_iss_typical(veh_obj_reference) :

  # Set the translational position.
  veh_obj_reference.trans_init.dyn_subject = veh_obj_reference.body
  veh_obj_reference.trans_init.reference_ref_frame_name = "Earth.inertial"
  veh_obj_reference.trans_init.body_frame_id     = "composite_body"
  veh_obj_reference.trans_init.position  = [ -4292653.41, 955168.47, 5139356.57]
  veh_obj_reference.trans_init.velocity  = [ 109.649663, -7527.726490, 1484.521489]

  # Set the rotational position.
  veh_obj_reference.lvlh_init.subject                    = veh_obj_reference.body.mass
  veh_obj_reference.lvlh_init.planet_name                = "Earth"
  veh_obj_reference.lvlh_init.body_frame_id              = "composite_body"
  veh_obj_reference.lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
  veh_obj_reference.lvlh_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
  veh_obj_reference.lvlh_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, -11.6, 0.0])
  veh_obj_reference.lvlh_init.ang_velocity = [ 0.0, 0.0, 0.0]
