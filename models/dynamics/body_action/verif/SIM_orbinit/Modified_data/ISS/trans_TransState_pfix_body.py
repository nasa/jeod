################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_ISS_trans_TransState_pfix_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify known initial planet-fixed translational state f ISS
#   at STS_114 MET 001:19:30:59.000.
#   Data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_ISS_trans_TransState_pfix_body(vehicle_reference, dynamics) :




  vehicle_reference.trans_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.trans_init.action_name = "ISS.trans_TransState_pfix_body"

  vehicle_reference.trans_init.reference_ref_frame_name = "Earth.pfix"
  vehicle_reference.trans_init.body_frame_id = "composite_body"

  vehicle_reference.trans_init.position  = trick.attach_units( "m",[     5406298.5700, -2074684.5600,  3426540.0300])


  vehicle_reference.trans_init.velocity  = trick.attach_units( "m/s",[     -706.0655810,  5764.3071620,  4587.2461630])





  dynamics.dyn_manager.add_body_action (vehicle_reference.trans_init)

  return
