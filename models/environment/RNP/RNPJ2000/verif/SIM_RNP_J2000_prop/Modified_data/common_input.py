trick.sim_services.exec_set_trap_sigfpe(1)
exec(compile(open("Log_data/log_verif_rec.py", "rb").read(), "Log_data/log_verif_rec.py", 'exec'))

jeod_time.time_manager_init.initializer = "UTC"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar
jeod_time.time_tai.update_from_name = "Dyn"
jeod_time.time_tai.initialize_from_name = "UTC"

jeod_time.time_tt.update_from_name = "TAI"
jeod_time.time_tt.initialize_from_name = "TAI"

jeod_time.time_ut1.update_from_name = "TAI"
jeod_time.time_ut1.initialize_from_name = "TAI"

jeod_time.time_gmst.update_from_name = "UT1"
jeod_time.time_gmst.initialize_from_name = "UT1"

jeod_time.time_utc.update_from_name = "TAI"
jeod_time.time_ut1.true_ut1 = False
jeod_time.time_utc.true_utc = False



# Set up the dynamics manager
dynamics.dyn_manager_init.central_point_name = "Earth"
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_2

trick.stop(1.0)

#***************************************************************
def set_utc_time( year, month, day, hour, minute, seconds) :
  jeod_time.time_utc.calendar_year  = year
  jeod_time.time_utc.calendar_month = month
  jeod_time.time_utc.calendar_day   = day
  jeod_time.time_utc.calendar_hour  = hour
  jeod_time.time_utc.calendar_minute   = minute
  jeod_time.time_utc.calendar_second   = seconds
