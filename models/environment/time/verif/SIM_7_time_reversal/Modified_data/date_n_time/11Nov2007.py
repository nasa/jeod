#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (Setup the simulation initial time and time model parameters.)
#
#REFERENCE:
#   (Trick Orbital Dynamics package Time documentation.)
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Time is input in UTC.))
#
# 
#*/

def set_date_n_time_11Nov2007(jeod_time) :

  #// Initialize from UTC calendar date.
  jeod_time.manager_init.initializer = "UTC"
  jeod_time.manager_init.sim_start_format = trick.TimeEnum.calendar

  #// Time initialization data.
  #// Midnight on November 20, 2007.
  jeod_time.utc.calendar_year   = 2007
  jeod_time.utc.calendar_month  =   11
  jeod_time.utc.calendar_day    =   20
  jeod_time.utc.calendar_hour   =    0
  jeod_time.utc.calendar_minute =    0
  jeod_time.utc.calendar_second =  0.0

  jeod_time.tai.initialize_from_name = "UTC"
  jeod_time.ut1.initialize_from_name = "TAI"
  jeod_time.tt.initialize_from_name  = "TAI"
  jeod_time.gmst.initialize_from_name  = "UT1"

  jeod_time.tai.update_from_name = "Dyn"
  jeod_time.ut1.update_from_name = "TAI"
  jeod_time.utc.update_from_name = "TAI"
  jeod_time.tt.update_from_name  = "TAI"
  jeod_time.gmst.update_from_name = "UT1"

  jeod_time.utc.true_utc = False
  jeod_time.ut1.true_ut1 = False

  #// Override the time computation parameters.
  jeod_time.conv_tai_utc.override_data_table = True
  jeod_time.conv_tai_utc.leap_sec_override_val = 32

  jeod_time.conv_tai_ut1.override_data_table = True
  jeod_time.conv_tai_ut1.tai_to_ut1_override_val = -32.469


  return
