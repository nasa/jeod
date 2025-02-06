################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_STS14_chaser_rate_def
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#
#Purpose:
#  (Specify STS-114 relative attitude rate.
#   Data are invented.)
#
#
#
#*******************************************************************************/

# def set_STS_114_chaser_rate_def(w_sts_lvlh, w_sts_issb, w_iss_lvlh) :
def set_STS_114_chaser_rate_def(w_iss_lvlh) :


  w_sts_lvlh  = [ 0.06,  0.03, 0.02]



  return w_sts_lvlh, [w_sts_lvlh[0] - w_iss_lvlh[0], w_sts_lvlh[1] - w_iss_lvlh[1], w_sts_lvlh[2] - w_iss_lvlh[2]]
