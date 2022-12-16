################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_tai_initialized/ 
####################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_rec.py", "rb").read(), "Log_data/log_rec.py", 'exec'))
log_rec( LOG_CYCLE )

#############  ASSIGNMENTS, CALLS, and CONTROL  #################

jeod_time.manager_init.initializer = "TAI"
jeod_time.manager_init.sim_start_format = trick.TimeEnum.calendar

jeod_time.tai.calendar_year = 2005
jeod_time.tai.calendar_month = 12
jeod_time.tai.calendar_day = 31
jeod_time.tai.calendar_hour = 23
jeod_time.tai.calendar_minute = 59
jeod_time.tai.calendar_second = 50.0

jeod_time.tai.update_from_name = "Dyn"
jeod_time.new_time.initialize_from_name = "TAI"
jeod_time.new_time.update_from_name = "TAI"


trick.sim_services.exec_set_terminate_time(25)
