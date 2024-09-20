################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_scale_factor_changes/ 
####################################################################################

#Auto-generated from the data file with this header:
#/*******************************************************************************
#*  This input file runs the test of whether the dynamic time can handle changes*
#*  to its advance rate (w.r.t. simtime), including a complete time reversal.   *
#*******************************************************************************/


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


trick.add_read(5,  """
jeod_time.time_manager.dyn_time.scale_factor = -1.0
""")

trick.add_read(10,  """
jeod_time.time_manager.dyn_time.scale_factor = 0.5
""")

trick.add_read(20,  """
jeod_time.time_manager.dyn_time.scale_factor = -2
""")

trick.sim_services.exec_set_terminate_time(25)
