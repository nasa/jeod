def set_trans_init_typical() :
  vehicle.trans_init.position  = [ -4292653.41, 955168.47, 5139356.57]
  vehicle.trans_init.velocity  = [ 109.649663, -7527.726490, 1484.521489]


def set_trans_init_elliptical() :
  vehicle.trans_init.position  = [ -4315967.74, 960356.20, 5167269.53]
  vehicle.trans_init.velocity  = [ 129.091037, -7491.513855, 1452.515654]


def set_rot_rate_inrtl():
  # Set initial attitude off LVLH but initial attitude rate off inertial
  # Restrict lvlh-init to be attitude only, and add rot-init
  vehicle.lvlh_init.set_items = trick.RefFrameItems.Att

  vehicle.rot_init.state_items = trick.DynBodyInitRotState.Rate
  vehicle.rot_init.set_subject_body( vehicle.dyn_body )
  vehicle.rot_init.reference_ref_frame_name = "Earth.inertial"
  vehicle.rot_init.body_frame_id = "composite_body"
  vehicle.rot_init.ang_velocity  = [ 0.0, 0.0, 0.0]

  dynamics.dyn_manager.add_body_action(vehicle.rot_init)



# Set the orientation by LVLH
def set_orientation_lvlh():
  vehicle.lvlh_init.set_subject_body( vehicle.dyn_body )
  vehicle.lvlh_init.planet_name                = "Earth"
  vehicle.lvlh_init.body_frame_id              = "composite_body"
  vehicle.lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
  vehicle.lvlh_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
  vehicle.lvlh_init.orientation.euler_angles   = trick.attach_units( "degree",[ 0.0, -11.6, 0.0])
  vehicle.lvlh_init.ang_velocity               = [ 0.0, 0.0, 0.0]

  dynamics.dyn_manager.add_body_action(vehicle.lvlh_init)





# Set derived state configurations
vehicle.pfix.reference_name     = "Earth"
vehicle.lvlh.reference_name     = "Earth"
vehicle.orb_elem.reference_name = "Earth"
vehicle.lvlh_euler.sequence     = trick.Orientation.Pitch_Yaw_Roll


# Set the trans state
vehicle.trans_init.set_subject_body( vehicle.dyn_body )
vehicle.trans_init.reference_ref_frame_name = "Earth.inertial"
vehicle.trans_init.body_frame_id            = "composite_body"
#default to "typical" trans-state and lvlh-based orientation
set_trans_init_typical()
set_orientation_lvlh()

dynamics.dyn_manager.add_body_action( vehicle.trans_init)
