################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_initialize_by_calendar/ 
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
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

jeod_time.time_tai.calendar_year = 2005
jeod_time.time_tai.calendar_month = 12
jeod_time.time_tai.calendar_day = 31
jeod_time.time_tai.calendar_hour = 23
jeod_time.time_tai.calendar_minute = 59
jeod_time.time_tai.calendar_second = 50.0

jeod_time.time_tai.update_from_name = "Dyn"


trick.sim_services.exec_set_terminate_time(25)
