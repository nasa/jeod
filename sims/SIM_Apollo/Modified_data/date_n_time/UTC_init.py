#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (Setup the time scales and have them initialize from UTC.)
#
#REFERENCE:
#   (Trick Orbital Dynamics package Time documentation.)
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Time is input in UTC.))
#
# 
#*/

def set_date_n_time_UTC_init(jeod_time) :

  #// Initialize from UTC calendar date.
  jeod_time.time_manager_init.initializer = "UTC"
  jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

  jeod_time.time_tai.initialize_from_name = "UTC"
  jeod_time.time_ut1.initialize_from_name = "TAI"
  jeod_time.time_tt.initialize_from_name  = "TAI"
  jeod_time.time_gmst.initialize_from_name = "UT1"

  jeod_time.time_met.update_from_name = "TAI"
  jeod_time.time_tai.update_from_name = "Dyn"
  jeod_time.time_ut1.update_from_name = "TAI"
  jeod_time.time_utc.update_from_name = "TAI"
  jeod_time.time_tt.update_from_name  = "TAI"
  jeod_time.time_gmst.update_from_name = "UT1"
