################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_rot_NedRotState_tned_struct_pad_39a
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

def set_STS_114_rot_NedRotState_tned_struct_pad_39a(vehicle_reference, dynamics) :
  vehicle_reference.ned_att_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.ned_att_init.action_name = "STS_114.rot_NedRotState_tned_struct_pad_39a"

  vehicle_reference.ned_att_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.ned_att_init.body_frame_id = "structure"
  vehicle_reference.ned_att_init.planet_name = "Earth"
  vehicle_reference.ned_att_init.ref_body_name = "PAD_39A"
  vehicle_reference.ned_att_init.altlatlong_type = trick.NorthEastDown.elliptical

  vehicle_reference.ned_att_init.set_items = trick.RefFrameItems.Att_Rate
  vehicle_reference.ned_att_init.orientation.data_source = trick.Orientation.InputEulerRotation
  vehicle_reference.ned_att_init.orientation.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  vehicle_reference.ned_att_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, 0.0, 0.0])
  vehicle_reference.ned_att_init.ang_velocity  = trick.attach_units( "degree/s",[ 1.0, 0.0, 0.0])

  dynamics.dyn_manager.add_body_action (vehicle_reference.ned_att_init)
