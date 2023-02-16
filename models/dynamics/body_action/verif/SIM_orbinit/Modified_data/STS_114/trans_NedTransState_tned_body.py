################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_NedTransState_tned_body
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

def set_STS_114_trans_NedTransState_tned_body(vehicle_reference, dynamics) :




  vehicle_reference.ned_trans_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.ned_trans_init.action_name = "STS_114.trans_NedTransState_tned_body"

  vehicle_reference.ned_trans_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.ned_trans_init.body_frame_id = "composite_body"
  vehicle_reference.ned_trans_init.planet_name = "Earth"
  vehicle_reference.ned_trans_init.ref_body_name = "ISS"
  vehicle_reference.ned_trans_init.altlatlong_type = trick.NorthEastDown.spherical

  vehicle_reference.ned_trans_init.position  = trick.attach_units( "m",[    17.504, 17.914, 126.613])


  vehicle_reference.ned_trans_init.velocity  = trick.attach_units( "m/s",[    -0.101060, -0.095858, -0.972466])





  dynamics.dyn_manager.add_body_action (vehicle_reference.ned_trans_init)

  return
