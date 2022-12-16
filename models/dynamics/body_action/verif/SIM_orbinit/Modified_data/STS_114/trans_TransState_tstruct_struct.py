################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_TransState_tstruct_struct
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

def set_STS_114_trans_TransState_tstruct_struct(vehicle_reference, dynamics) :




  vehicle_reference.trans_init.dyn_subject = vehicle_reference.dyn_body
  vehicle_reference.trans_init.action_name = "STS_114.trans_TransState_tstruct_struct"

  vehicle_reference.trans_init.reference_ref_frame_name = "ISS.structure"
  vehicle_reference.trans_init.body_frame_id = "structure"

  vehicle_reference.trans_init.position = trick.attach_units("m", [9.844 +   5 + 9.600,
                                                                   0.000 +   0 + 0.003,
                                                                   5.252 + 100 - 3.937])




  vehicle_reference.trans_init.velocity  = trick.attach_units( "m/s",[    0.0, 0.0, -1.0])





  dynamics.dyn_manager.add_body_action (vehicle_reference.trans_init)

  return
