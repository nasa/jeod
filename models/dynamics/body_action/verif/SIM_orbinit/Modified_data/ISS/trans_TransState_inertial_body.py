################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_ISS_trans_TransState_inertial_body
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify known initial inertial translational state f ISS
#   at STS_114 MET 001:19:30:59.000.
#   Data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_ISS_trans_TransState_inertial_body(vehicle_reference, dynamics) :



  vehicle_reference.trans_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.trans_init.action_name = "ISS.trans_TransState_inertial_body"

  vehicle_reference.trans_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.trans_init.body_frame_id = "composite_body"

  vehicle_reference.trans_init.position  = trick.attach_units( "m",[     1244540.5300,  5655938.8500,  3425643.2200])


  vehicle_reference.trans_init.velocity  = trick.attach_units( "m/s",[    -6003.8330510, -1469.4960440,  4590.5117760])





  dynamics.dyn_manager.add_body_action (vehicle_reference.trans_init)

  return
