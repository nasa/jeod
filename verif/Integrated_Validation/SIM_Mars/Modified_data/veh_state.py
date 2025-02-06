# All instances have the same lvlh orientation state.
dawn.lvlh_init.set_subject_body( dawn.dyn_body )
dawn.lvlh_init.planet_name                = "Mars"
dawn.lvlh_init.body_frame_id              = "composite_body"
dawn.lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
dawn.lvlh_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
dawn.lvlh_init.orientation.euler_angles   = [ 0.0, 0.0, 0.0]
dawn.lvlh_init.ang_velocity               = [ 0.0, 0.0, 0.0]
dynamics.dyn_manager.add_body_action (dawn.lvlh_init)

def set_dawn_trans_state() :
  # State initialization data for a Dawn orbital state
  # Set the translational position.
  dawn.trans_init.set_subject_body( dawn.dyn_body )
  dawn.trans_init.reference_ref_frame_name = "Mars.inertial"
  dawn.trans_init.body_frame_id     = "composite_body"
  dawn.trans_init.position  = trick.attach_units( "km",[ 11563.3556802 , -14356.6688977   , 6293.7046169    ])
  dawn.trans_init.velocity  = trick.attach_units( "km/s",[ -2.2731078  ,     2.3801324    ,   -.0229110 ])
  dynamics.dyn_manager.add_body_action (dawn.trans_init)

def set_phobos_trans_state() :
  # State initialization data for a Phobos orbital state
  # Set the translational position.
  dawn.trans_init.set_subject_body( dawn.dyn_body )
  dawn.trans_init.reference_ref_frame_name = "Mars.inertial"
  dawn.trans_init.body_frame_id     = "composite_body"
  dawn.trans_init.position  = trick.attach_units( "km",[ 8240.7901108 , 605.0716371   , -4152.5375845 ])
  dawn.trans_init.velocity  = trick.attach_units( "km/s",[ 0.3077392  ,     1.9627295    ,   0.8655744 ])
  dynamics.dyn_manager.add_body_action (dawn.trans_init)

def set_phobos_orb_state() :
  # State initialization data for a Phobos orbital state
  dawn.orb_init.set_subject_body( dawn.dyn_body )
  dawn.orb_init.reference_ref_frame_name = "Mars.inertial"
  dawn.orb_init.body_frame_id    = "composite_body"
  dawn.orb_init.planet_name      = "Mars"
  dawn.orb_init.orbit_frame_name = "Mars.alt_inertial"
  dawn.orb_init.set             = 10
  dawn.orb_init.semi_major_axis = 9379352 # meters
  dawn.orb_init.eccentricity    = 0.015445
  dawn.orb_init.inclination     = 0.018948 #radians
  dawn.orb_init.arg_periapsis   = 0.313171 #radians
  dawn.orb_init.ascending_node  = 1.580166 #radians
  dawn.orb_init.true_anomaly    = 0.43727  #radians
  dynamics.dyn_manager.add_body_action(dawn.orb_init)
