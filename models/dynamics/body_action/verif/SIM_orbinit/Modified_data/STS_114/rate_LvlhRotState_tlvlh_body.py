################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_rate_LvlhRotState_tlvlh_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify STS-114 relative attitude rate.
#   Data are invented.)
#
#
#
#*******************************************************************************/

exec(compile(open("Modified_data/STS_114/chaser_rate_def.py", "rb").read(), "Modified_data/STS_114/chaser_rate_def.py", 'exec'))
exec(compile(open("Modified_data/STS_114/chaser_rate_def.py", "rb").read(), "Modified_data/STS_114/chaser_rate_def.py", 'exec'))

def set_STS_114_rate_LvlhRotState_tlvlh_body(vehicle_reference, dynamics) :


  w_sts_lvlh = [0, 0, 0]
  w_sts_issb = [0, 0, 0]
  w_iss_lvlh = [0, 0, 0]
  w_iss_lvlh = set_ISS_iss_rate_def()

  # set_STS_114_chaser_rate_def(w_sts_lvlh, w_sts_issb, w_iss_lvlh)
  w_sts_lvlh, w_sts_issb = set_STS_114_chaser_rate_def(w_iss_lvlh)

  vehicle_reference.lvlh_rate_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.lvlh_rate_init.action_name = "STS_114.rate_LvlhRotState_tlvlh_body"

  vehicle_reference.lvlh_rate_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.lvlh_rate_init.body_frame_id = "composite_body"
  vehicle_reference.lvlh_rate_init.planet_name = "Earth"
  vehicle_reference.lvlh_rate_init.ref_body_name = "ISS"

  vehicle_reference.lvlh_rate_init.set_items = trick.RefFrameItems.Rate

  vehicle_reference.lvlh_rate_init.ang_velocity  = trick.attach_units( "degree/s",[    -w_sts_lvlh[2], w_sts_lvlh[1], w_sts_lvlh[0]])




  dynamics.dyn_manager.add_body_action (vehicle_reference.lvlh_rate_init)

  return
