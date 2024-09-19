################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_dyn/ 
####################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_rec.py", "rb").read(), "Log_data/log_rec.py", 'exec'))
log_rec( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################
#define LOG_CYCLE 1.0

#include "S_default.dat"
#include "Log_data/rec.d"


trick.sim_services.exec_set_terminate_time(25)
