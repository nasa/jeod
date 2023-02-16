# Set the translational position.
def state_iss_highly_elliptical():
  sv_dyn.trans_init.position  = [ -4315967.74, 960356.20, 5167269.53]
  sv_dyn.trans_init.velocity  = [ 129.091037, -7491.513855, 1452.515654]

def state_iss_typical():
  sv_dyn.trans_init.position  = [ -4292653.41, 955168.47, 5139356.57]
  sv_dyn.trans_init.velocity  = [ 109.649663, -7527.726490, 1484.521489]


sv_dyn.trans_init.set_subject_body( sv_dyn.dyn_body.mass )
sv_dyn.trans_init.reference_ref_frame_name = "Earth.inertial"
sv_dyn.trans_init.body_frame_id     = "composite_body"
dynamics.dyn_manager.add_body_action(sv_dyn.trans_init)


# Set the rotational position.
sv_dyn.lvlh_init.set_subject_body( sv_dyn.dyn_body )
sv_dyn.lvlh_init.planet_name               = "Earth"
sv_dyn.lvlh_init.body_frame_id             = "composite_body"
sv_dyn.lvlh_init.orientation.data_source   = trick.Orientation.InputEulerRotation
sv_dyn.lvlh_init.orientation.euler_sequence= trick.Orientation.Yaw_Pitch_Roll
sv_dyn.lvlh_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, -11.6, 0.0])
sv_dyn.lvlh_init.ang_velocity              = [ 0.0, 0.0, 0.0]
dynamics.dyn_manager.add_body_action(sv_dyn.lvlh_init)
