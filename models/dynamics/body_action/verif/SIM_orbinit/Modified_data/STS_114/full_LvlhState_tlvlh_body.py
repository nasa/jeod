################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_full_LvlhState_tlvlh_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify ISS-relative STS-114 state
#   at STS_114 MET 001:19:30:59.000.
#   Data based on data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

exec(compile(open("Modified_data/STS_114/chaser_rate_def.py", "rb").read(), "Modified_data/STS_114/chaser_rate_def.py", 'exec'))
exec(compile(open("Modified_data/ISS/iss_rate_def.py", "rb").read(), "Modified_data/ISS/iss_rate_def.py", 'exec'))

def set_STS_114_full_LvlhState_tlvlh_body(vehicle_reference, dynamics) :


  w_sts_lvlh = [0, 0, 0]
  w_sts_issb = [0, 0, 0]
  w_iss_lvlh = [0, 0, 0]
  w_iss_lvlh = set_ISS_iss_rate_def()


  # set_STS_114_chaser_rate_def(w_sts_lvlh, w_sts_issb, w_iss_lvlh)
  w_sts_lvlh, w_sts_issb = set_STS_114_chaser_rate_def(w_iss_lvlh)


  vehicle_reference.lvlh_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.lvlh_init.action_name = "STS_114.full_LvlhState_tlvlh_body"

  vehicle_reference.lvlh_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.lvlh_init.body_frame_id = "composite_body"
  vehicle_reference.lvlh_init.planet_name = "Earth"
  vehicle_reference.lvlh_init.ref_body_name = "ISS"
  vehicle_reference.lvlh_init.set_items = trick.RefFrameItems.Pos_Vel_Att_Rate

  vehicle_reference.lvlh_init.position = trick.attach_units("m",[10.201 + 9.844 + 5 + 9.600 -  9.600,
                                                                 -0.206 + 0.000 +   0 + 0.003 -  0.003,
                                                                 -2.558 + 5.252 + 100 - 3.937 + 27.856])




  vehicle_reference.lvlh_init.velocity  = trick.attach_units( "m/s",[    0.0, 0.0, -1.0])



  vehicle_reference.lvlh_init.orientation.data_source = trick.Orientation.InputEulerRotation
  vehicle_reference.lvlh_init.orientation.euler_sequence = trick.Orientation.Pitch_Roll_Yaw
  vehicle_reference.lvlh_init.orientation.euler_angles  = trick.attach_units( "degree",[    90.0, 0.0, 0.0])



  vehicle_reference.lvlh_init.ang_velocity  = trick.attach_units( "degree/s",[    -w_sts_lvlh[2], w_sts_lvlh[1], w_sts_lvlh[0]])





  dynamics.dyn_manager.add_body_action (vehicle_reference.lvlh_init)

  return
