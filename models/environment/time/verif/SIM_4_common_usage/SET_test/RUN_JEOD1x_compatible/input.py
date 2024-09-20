################TRICK HEADER#######################################
#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_JEOD1x_compatible/ 
####################################################################################


trick.sim_services.exec_set_trap_sigfpe(1)

#############  NUMERICAL DEFAULT DATA  #################
LOG_CYCLE = 1.0

#############  LOG DATA  #################
exec(compile(open( "Log_data/log_rec.py", "rb").read(), "Log_data/log_rec.py", 'exec'))
log_rec( LOG_CYCLE )

#############  MODIFIED DATA  #################

#############  ASSIGNMENTS, CALLS, and CONTROL  #################


jeod_time.time_manager_init.initializer = "UTC"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

#//This is just before a leap second.  TAI should be smooth, UTC will show
#//discontinuities.
#// But set UT1 and UTC to be calculated the same as in JEOD 1.x
jeod_time.time_utc.true_utc = False
jeod_time.time_ut1.true_ut1 = False


jeod_time.time_utc.calendar_year = 1998
jeod_time.time_utc.calendar_month = 12
jeod_time.time_utc.calendar_day = 31
jeod_time.time_utc.calendar_hour = 0
jeod_time.time_utc.calendar_minute = 0
jeod_time.time_utc.calendar_second = 0.0

jeod_time.time_tai.update_from_name = "Dyn"
jeod_time.time_tai.initialize_from_name = "UTC"
jeod_time.time_utc.update_from_name = "TAI"
jeod_time.time_ut1.initialize_from_name = "TAI"
jeod_time.time_ut1.update_from_name = "TAI"




trick.sim_services.exec_set_terminate_time(86500)
