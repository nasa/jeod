################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_state_sv_leo_lvlh 
####################################################################################

#Auto-generated from the data file with this header:
#// State initialization data for a typical low Earth orbit.

def set_state_sv_leo_lvlh(sv_dyn_reference) :

  #//
  #// Set the translational position.
  #//
  sv_dyn_reference.trans_init.set_subject_body( sv_dyn_reference.dyn_body )
  sv_dyn_reference.trans_init.reference_ref_frame_name = "Earth.inertial"
  sv_dyn_reference.trans_init.body_frame_id     = "composite_body"
  sv_dyn_reference.trans_init.position  = [ -4292653.41, 955168.47, 5139356.57]
  sv_dyn_reference.trans_init.velocity  = [ 109.649663, -7527.726490, 1484.521489]

  #//
  #// Set the rotational position.
  #//
  sv_dyn_reference.lvlh_init.set_subject_body( sv_dyn_reference.dyn_body )
  sv_dyn_reference.lvlh_init.planet_name                     = "Earth"
  sv_dyn_reference.lvlh_init.body_frame_id                   = "composite_body"
  sv_dyn_reference.lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
  sv_dyn_reference.lvlh_init.orientation.euler_sequence      = trick.Orientation.Yaw_Pitch_Roll
  sv_dyn_reference.lvlh_init.orientation.euler_angles  = [ 0.0, 0.0, 0.0]
  sv_dyn_reference.lvlh_init.ang_velocity              = [ 0.0, 0.0, 0.0]
