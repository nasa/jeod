def veh_state( veh):
  # Initialize Translational state
  veh.init_orbit.set_subject_body( veh.dyn_body.mass )
  veh.init_orbit.reference_ref_frame_name = "Earth.pfix"
  veh.init_orbit.orbit_frame_name = "Earth.pfix"
  veh.init_orbit.planet_name = "Earth"
  veh.init_orbit.body_frame_id = "composite_body"
  veh.init_orbit.set = trick.DynBodyInitOrbit.IncAscnodeAltperAltapoArgperTanom #4

  veh.init_orbit.inclination    = 0.0
  veh.init_orbit.ascending_node = 0.0
  veh.init_orbit.true_anomaly   = 0.0
  veh.init_orbit.alt_apoapsis   = trick.attach_units( "km",400.0)
  veh.init_orbit.alt_periapsis  = trick.attach_units( "km",400.0)

  dynamics.dyn_manager.add_body_action (veh.init_orbit)


  # Initialize Rotational state
  veh.init_lvlh.planet_name = "Earth"
  veh.init_lvlh.set_subject_body( veh.dyn_body )
  veh.init_lvlh.reference_ref_frame_name = "Earth.inertial"
  veh.init_lvlh.body_frame_id = "composite_body"

  veh.init_lvlh.orientation.data_source = trick.Orientation.InputEulerRotation
  veh.init_lvlh.orientation.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  veh.init_lvlh.orientation.euler_angles   = [ 0.0, 0.0, 0.0]
  veh.init_lvlh.ang_velocity               = [ 0.0, 0.0, 0.0]

  dynamics.dyn_manager.add_body_action (veh.init_lvlh)


def veh_state_alt(veh) :
  # Initialize Translational state
  veh.trans_init.set_subject_body( veh.dyn_body )
  veh.trans_init.reference_ref_frame_name = str(veh.dyn_body.integ_frame_name)
  veh.trans_init.body_frame_id     = "composite_body"
  veh.trans_init.position  = [ 0.0, 0.0, 0.0]
  veh.trans_init.velocity  = [ 0.0, 0.0, 0.0]

  dynamics.dyn_manager.add_body_action(veh.trans_init)


  # Initialize Rotational state
  veh.rot_init.set_subject_body( veh.dyn_body )
  veh.rot_init.reference_ref_frame_name = str(veh.dyn_body.integ_frame_name)
  veh.rot_init.body_frame_id            = "composite_body"
  veh.rot_init.orientation.data_source  = trick.Orientation.InputQuaternion
  veh.rot_init.ang_velocity             = [ 0.0, 0.0, 0.0]

  dynamics.dyn_manager.add_body_action(veh.rot_init)
