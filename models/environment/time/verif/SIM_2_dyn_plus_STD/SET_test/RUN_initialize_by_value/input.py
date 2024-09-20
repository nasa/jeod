################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_initialize_by_value/ 
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

jeod_time.time_manager_init.initializer = "TAI"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.truncated_julian
#//time.time_manager_init.sim_start_format = trick.TimeEnum.modified_julian
#//time.time_manager_init.sim_start_format = trick.TimeEnum.julian
#//time.time_manager_init.sim_start_format = trick.TimeEnum.seconds_since_epoch
#//time.time_manager_init.sim_start_format = trick.TimeEnum.days_since_epoch


jeod_time.time_tai.initializing_value = 10000

jeod_time.time_tai.update_from_name = "Dyn"


trick.sim_services.exec_set_terminate_time(25)
