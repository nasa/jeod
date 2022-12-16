################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_TransState_tpoint_point
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify ISS-relative STS-114 translational state
#   at STS_114 MET 001:19:30:59.000.
#   Data based on data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_STS_114_trans_TransState_tpoint_point(vehicle_reference, dynamics) :




  vehicle_reference.trans_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.trans_init.action_name = "STS_114.trans_TransState_tpoint_point"

  vehicle_reference.trans_init.reference_ref_frame_name = "ISS.attach_point"
  vehicle_reference.trans_init.body_frame_id = "attach_point"

  vehicle_reference.trans_init.position  = trick.attach_units( "m",[ 100.0, 0.0, 5.0])
  vehicle_reference.trans_init.velocity  = trick.attach_units( "m/s",[ -1.0, 0.0, 0.0])



  dynamics.dyn_manager.add_body_action (vehicle_reference.trans_init)

  return
