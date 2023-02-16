################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_att_RotState_tpoint_point
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

def set_STS_114_att_RotState_tpoint_point(vehicle_reference, dynamics) :
  vehicle_reference.att_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.att_init.action_name = "STS_114.att_RotState_tpoint_point"

  vehicle_reference.att_init.reference_ref_frame_name = "ISS.attach_point"
  vehicle_reference.att_init.body_frame_id = "attach_point"

  vehicle_reference.att_init.state_items = trick.DynBodyInitRotState.Attitude
  vehicle_reference.att_init.orientation.data_source = trick.Orientation.InputEulerRotation
  vehicle_reference.att_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
  vehicle_reference.att_init.orientation.euler_angles  = trick.attach_units( "degree",[ 180.0, 0.0, 0.0])

  dynamics.dyn_manager.add_body_action (vehicle_reference.att_init)
