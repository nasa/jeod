################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_fail_dyn_initialized/ 
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
#FIXME:  sys.exec.in.trap_sigfpe          = Yes
#FIXME:  sys.exec.in.pre_init_checkpoint  = No
#FIXME:  sys.exec.in.post_init_checkpoint = No
#FIXME:  sys.exec.in.end_checkpoint       = No
#FIXME:  sys.exec.in.reduced_checkpoint   = No
#FIXME:  sys.exec.sim_com.monitor_on      = No
#FIXME:  sys.exec.rt_log.group[0].record  = No

#// Try to initialize from Dyn
jeod_time.time_manager_init.initializer = "Dyn"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.truncated_julian

jeod_time.time_tai.trunc_julian_time = 10000

jeod_time.time_tai.update_from_name = "Dyn"


trick.sim_services.exec_set_terminate_time(25)