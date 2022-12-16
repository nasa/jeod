################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_ISS_reference_inertial_trans_state
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify known initial inertial translational state for the ISS body frame
#   at STS_114 MET 001:19:30:59.000.
#   Data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_ISS_reference_inertial_trans_state(vehicle_reference) :


  vehicle_reference.expected_state.trans.position  = [      1244540.53,   5655938.85,   3425643.22]


  vehicle_reference.expected_state.trans.velocity  = [    -6003.833051, -1469.496044,  4590.511776]



  return
