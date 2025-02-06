################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_TransState_tbody_body
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

def set_STS_114_trans_TransState_tbody_body(vehicle_reference, dynamics) :




  vehicle_reference.trans_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.trans_init.action_name = "STS_114.trans_TransState_tbody_body"

  vehicle_reference.trans_init.reference_ref_frame_name = "ISS.composite_body"
  vehicle_reference.trans_init.body_frame_id = "composite_body"

  vehicle_reference.trans_init.position = trick.attach_units("m", [10.201 + 9.844 +   5 + 9.600 -  9.600,
                                                                   -0.206 + 0.000 +   0 + 0.003 -  0.003,
                                                                   -2.558 + 5.252 + 100 - 3.937 + 27.856])




  vehicle_reference.trans_init.velocity  = trick.attach_units( "m/s",[    0.0, 0.0, -1.0])





  dynamics.dyn_manager.add_body_action (vehicle_reference.trans_init)

  return
