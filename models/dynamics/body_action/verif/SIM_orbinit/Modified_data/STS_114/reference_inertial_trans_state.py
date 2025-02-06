################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_reference_inertial_trans_state
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify known initial inertial translational state for the STS-114 body frame
#   at STS_114 MET 001:19:30:59.000.
#   Data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_STS_114_reference_inertial_trans_state(vehicle_reference) :


  vehicle_reference.expected_state.trans.position  = trick.attach_units( "m",[      1244471.94,   5655811.80,   3425518.88])


  vehicle_reference.expected_state.trans.velocity  = trick.attach_units( "m/s",[    -6003.553468, -1469.321965,  4590.577230])



  return
