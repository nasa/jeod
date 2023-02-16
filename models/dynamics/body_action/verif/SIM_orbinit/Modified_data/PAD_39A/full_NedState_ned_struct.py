################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_PAD_39A_full_NedState_ned_struct
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify PAD 39A reference state.
#   Data are invented.)
#
#
#
#*******************************************************************************/

def set_PAD_39A_full_NedState_ned_struct(vehicle_reference, dynamics) :


  vehicle_reference.ned_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.ned_init.action_name = "PAD_39A.full_NedState_ned_struct"

  vehicle_reference.ned_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.ned_init.body_frame_id = "structure"
  vehicle_reference.ned_init.planet_name = "Earth"

  vehicle_reference.ned_init.ref_point.altitude  = trick.attach_units( "m",3.0)
  vehicle_reference.ned_init.ref_point.latitude  = trick.attach_units( "degree",28.6082)
  vehicle_reference.ned_init.ref_point.longitude  = trick.attach_units( "degree",-80.6040)
  vehicle_reference.ned_init.altlatlong_type = trick.NorthEastDown.elliptical

  vehicle_reference.ned_init.set_items = trick.RefFrameItems.Pos_Vel_Att_Rate
  vehicle_reference.ned_init.position  = trick.attach_units( "m",[  0.0,  0.0, 10.0])
  vehicle_reference.ned_init.velocity  = trick.attach_units( "m/s",[  0.0,  0.0,  0.0])
  vehicle_reference.ned_init.orientation.data_source = trick.Orientation.InputEulerRotation
  vehicle_reference.ned_init.orientation.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  vehicle_reference.ned_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, 0.0, 0.0])
  vehicle_reference.ned_init.ang_velocity  = trick.attach_units( "degree/s",[ 0.0, 0.0, 0.0])



  dynamics.dyn_manager.add_body_action (vehicle_reference.ned_init)

  return
