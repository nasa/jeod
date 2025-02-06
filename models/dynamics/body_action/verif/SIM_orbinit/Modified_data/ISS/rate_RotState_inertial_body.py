################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_ISS_rate_RotState_inertial_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify ISS body frame attitude rate relative to inertial.
#   Data are invented.)
#
#
#
#*******************************************************************************/

exec(compile(open("Modified_data/ISS/iss_rate_def.py", "rb").read(), "Modified_data/ISS/iss_rate_def.py", 'exec'))
exec(compile(open("Modified_data/ISS/lvlh_rate_def.py", "rb").read(), "Modified_data/ISS/lvlh_rate_def.py", 'exec'))

def set_ISS_rate_RotState_inertial_body(vehicle_reference, dynamics) :

  w_iss_lvlh = [0, 0, 0]
  w_lvlh = 0

  w_iss_lvlh = set_ISS_iss_rate_def()
  w_lvlh = set_ISS_lvlh_rate_def()




  vehicle_reference.rate_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.rate_init.action_name = "ISS.rate_RotState_inertial_body"

  vehicle_reference.rate_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.rate_init.body_frame_id = "composite_body"

  vehicle_reference.rate_init.state_items = trick.DynBodyInitRotState.Rate
  vehicle_reference.rate_init.ang_velocity = trick.attach_units( "degree/s", [w_iss_lvlh[0], w_iss_lvlh[1] + w_lvlh, w_iss_lvlh[2]])







  dynamics.dyn_manager.add_body_action (vehicle_reference.rate_init)

  return
