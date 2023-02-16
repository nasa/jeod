################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_ISS_rot_LvlhRotState_lvlh_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify ISS body frame attitude and rate relative to ISS LVLH.
#   Data are invented.)
#
#
#*******************************************************************************/

exec(compile(open("Modified_data/ISS/iss_rate_def.py", "rb").read(), "Modified_data/ISS/iss_rate_def.py", 'exec'))

def set_ISS_rot_LvlhRotState_lvlh_body(vehicle_reference, dynamics) :


  w_iss_lvlh = [0, 0, 0]
  # set_ISS_iss_rate_def(w_iss_lvlh)
  w_iss_lvlh = set_ISS_iss_rate_def()



  vehicle_reference.lvlh_att_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.lvlh_att_init.action_name = "ISS.rot_LvlhRotState_lvlh_body"

  vehicle_reference.lvlh_att_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.lvlh_att_init.body_frame_id = "composite_body"
  vehicle_reference.lvlh_att_init.planet_name = "Earth"

  vehicle_reference.lvlh_att_init.set_items = trick.RefFrameItems.Att_Rate
  vehicle_reference.lvlh_att_init.orientation.data_source = trick.Orientation.InputEulerRotation
  vehicle_reference.lvlh_att_init.orientation.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  vehicle_reference.lvlh_att_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, 0.0, 0.0])
  vehicle_reference.lvlh_att_init.ang_velocity  = trick.attach_units( "degree/s",[    w_iss_lvlh[0], w_iss_lvlh[1], w_iss_lvlh[2]])

  dynamics.dyn_manager.add_body_action (vehicle_reference.lvlh_att_init)
