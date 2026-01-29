# Initialize from UTC calendar date.
jeod_time.time_manager_init.initializer = "UTC"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

jeod_time.time_tai.initialize_from_name = "UTC"
jeod_time.time_ut1.initialize_from_name = "TAI"
jeod_time.time_tt.initialize_from_name  = "TAI"
jeod_time.time_gmst.initialize_from_name  = "UT1"

jeod_time.time_tai.update_from_name = "Dyn"
jeod_time.time_ut1.update_from_name = "TAI"
jeod_time.time_utc.update_from_name = "TAI"
jeod_time.time_tt.update_from_name  = "TAI"
jeod_time.time_gmst.update_from_name  = "UT1"

def set_dawn_epoch():
  # Time initialization data.
  jeod_time.time_utc.set_date_and_time(2009, 2, 17, 23, 0, 0.0)

def set_phobos_epoch():
  jeod_time.time_utc.set_date_and_time(2010, 9, 10, 0, 0, 0.0)
