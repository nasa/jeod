#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (Setup the simulation initial simulation time.)
#
#REFERENCE:
#   (Trick Orbital Dynamics package Time documentation.)
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Time is input in UTC.))
#
# 
#*/

def set_date_n_time_UTC_16Jul1969(jeod_time) :

  #// Time initialization data.
  jeod_time.time_utc.calendar_year   = 1969
  jeod_time.time_utc.calendar_month  =    7
  jeod_time.time_utc.calendar_day    =   16
  jeod_time.time_utc.calendar_hour   =   13
  jeod_time.time_utc.calendar_minute =   44
  jeod_time.time_utc.calendar_second =  0.0

  #// Override the time computation parameters.
  #//The value here is from USNO historical data
  jeod_time.time_converter_tai_utc.override_data_table = True
  jeod_time.time_converter_tai_utc.leap_sec_override_val = 4.2

  jeod_time.time_converter_tai_ut1.override_data_table = True
  jeod_time.time_converter_tai_ut1.tai_to_ut1_override_val = 0.0115221 - 4.2
