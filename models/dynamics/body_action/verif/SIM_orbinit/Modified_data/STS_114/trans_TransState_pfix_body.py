################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_TransState_pfix_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify known initial planet-fixed translational state for the STS-114
#   at STS_114 MET 001:19:30:59.000.
#   Data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_STS_114_trans_TransState_pfix_body(vehicle_reference, dynamics) :




  vehicle_reference.trans_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.trans_init.action_name = "STS_114.trans_TransState_pfix_body"

  vehicle_reference.trans_init.reference_ref_frame_name = "Earth.pfix"
  vehicle_reference.trans_init.body_frame_id = "composite_body"

  vehicle_reference.trans_init.position  = trick.attach_units( "m",[  5406183.2100, -2074597.8000,  3426415.6500])
  vehicle_reference.trans_init.velocity  = trick.attach_units( "m/s",[  -705.9289500,  5764.0131340,  4587.3117740])



  dynamics.dyn_manager.add_body_action (vehicle_reference.trans_init)

  return
