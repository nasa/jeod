################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_ISS_att_RotState_inertial_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify ISS body frame attitude relative to inertial.
#   Data are invented.)
#
#
#
#*******************************************************************************/

def set_ISS_att_RotState_inertial_body(vehicle_reference, dynamics) :
  vehicle_reference.att_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.att_init.action_name = "ISS.att_RotState_inertial_body"

  vehicle_reference.att_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.att_init.body_frame_id = "composite_body"

  vehicle_reference.att_init.state_items = trick.DynBodyInitRotState.Attitude

  vehicle_reference.att_init.orientation.data_source = trick.Orientation.InputEulerRotation
  vehicle_reference.att_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
  vehicle_reference.att_init.orientation.euler_angles  = trick.attach_units( "degree",[    77.590713, -30.604895, -46.100115])

  dynamics.dyn_manager.add_body_action (vehicle_reference.att_init)
