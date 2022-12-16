################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_rot_RotState_tstruct_struct
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify STS-114 relative attitude.
#   Data are invented.)
#
#
#
#*******************************************************************************/

exec(compile(open("Modified_data/STS_114/chaser_rate_def.py", "rb").read(), "Modified_data/STS_114/chaser_rate_def.py", 'exec'))
exec(compile(open("Modified_data/ISS/iss_rate_def.py", "rb").read(), "Modified_data/ISS/iss_rate_def.py", 'exec'))

def set_STS_114_rot_RotState_tstruct_struct(vehicle_reference, dynamics) :
  w_sts_lvlh = [0, 0, 0]
  w_sts_issb = [0, 0, 0]
  w_iss_lvlh = [0, 0, 0]

  w_iss_lvlh = set_ISS_iss_rate_def()

  # set_STS_114_chaser_rate_def(w_sts_lvlh, w_sts_issb, w_iss_lvlh)
  w_sts_lvlh, w_sts_issb = set_STS_114_chaser_rate_def(w_iss_lvlh)

  vehicle_reference.att_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.att_init.action_name = "STS_114.rot_RotState_tstruct_struct"

  vehicle_reference.att_init.reference_ref_frame_name = "ISS.structure"
  vehicle_reference.att_init.body_frame_id = "structure"

  vehicle_reference.att_init.state_items = trick.DynBodyInitRotState.Both
  vehicle_reference.att_init.orientation.data_source = trick.Orientation.InputEulerRotation
  vehicle_reference.att_init.orientation.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  vehicle_reference.att_init.orientation.euler_angles  = trick.attach_units( "degree",[ -90.0, 0.0, 0.0])

  vehicle_reference.att_init.ang_velocity  = trick.attach_units( "degree/s",[-w_sts_issb[0], w_sts_issb[1], -w_sts_issb[2]])

  dynamics.dyn_manager.add_body_action (vehicle_reference.att_init)
