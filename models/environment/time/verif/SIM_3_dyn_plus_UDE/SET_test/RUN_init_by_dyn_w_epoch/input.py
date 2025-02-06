################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_init_by_dyn_w_epoch/ 
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


jeod_time.time_manager_init.initializer = "Dyn"

jeod_time.time_ude.epoch_defined_in_name = "Dyn"
jeod_time.time_ude.epoch_format = trick.TimeEnum.seconds_since_epoch
jeod_time.time_ude.set_epoch_initializing_value( trick.exec_get_sim_time(), 5)

jeod_time.time_ude.update_from_name = "Dyn"


trick.sim_services.exec_set_terminate_time(25)
