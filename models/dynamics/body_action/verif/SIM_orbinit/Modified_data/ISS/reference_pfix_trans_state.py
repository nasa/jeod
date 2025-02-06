################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_ISS_reference_pfix_trans_state
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify known initial planet-fixed translational state for the ISS body frame
#   at STS_114 MET 001:19:30:59.000.
#   Data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#*******************************************************************************/

def set_ISS_reference_pfix_trans_state(vehicle_reference) :


  vehicle_reference.expected_state.trans.position  = [      5406298.57,  -2074684.56,   3426540.03]


  vehicle_reference.expected_state.trans.velocity  = [     -706.065581,  5764.307162,  4587.246163]



  return
