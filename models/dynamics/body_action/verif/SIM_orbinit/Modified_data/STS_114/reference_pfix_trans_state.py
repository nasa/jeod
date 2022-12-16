################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS_114_reference_pfix_trans_state
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify known initial planet-fixed translational state for the STS-114 body
#   frame at STS_114 MET 001:19:30:59.000.
#   Data taken from
#   The Flight Operations Directorate, NASA JSC )
#
#
#
#*******************************************************************************/

def set_STS_114_reference_pfix_trans_state(vehicle_reference) :


  vehicle_reference.expected_state.trans.position  = trick.attach_units( "m",[      5406183.21,  -2074597.80,   3426415.65])


  vehicle_reference.expected_state.trans.velocity  = trick.attach_units( "m/s",[     -705.928950,  5764.013134,  4587.311774])



  return
