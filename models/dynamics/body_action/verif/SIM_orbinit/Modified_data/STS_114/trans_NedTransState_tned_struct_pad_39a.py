################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_trans_NedTransState_tned_struct_pad_39a
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify ISS-relative STS-114 translational state.
#   Data are invented.)
#
#
#
#*******************************************************************************/

def set_STS_114_trans_NedTransState_tned_struct_pad_39a(vehicle_reference, dynamics) :




  vehicle_reference.ned_trans_init.set_subject_body( vehicle_reference.dyn_body )
  vehicle_reference.ned_trans_init.action_name = "STS_114.trans_NedTransState_tned_struct_pad_39a"

  vehicle_reference.ned_trans_init.reference_ref_frame_name = "Earth.inertial"
  vehicle_reference.ned_trans_init.body_frame_id = "structure"
  vehicle_reference.ned_trans_init.planet_name = "Earth"
  vehicle_reference.ned_trans_init.ref_body_name = "PAD_39A"
  vehicle_reference.ned_trans_init.altlatlong_type = trick.NorthEastDown.elliptical

  vehicle_reference.ned_trans_init.position  = trick.attach_units( "m",[ 0.0, 0.0,  -40.0])
  vehicle_reference.ned_trans_init.velocity  = trick.attach_units( "m/s",[ 0.0, 0.0, -100.0])



  dynamics.dyn_manager.add_body_action (vehicle_reference.ned_trans_init)

  return
