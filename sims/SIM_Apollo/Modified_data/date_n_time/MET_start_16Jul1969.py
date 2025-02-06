#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#   (Setup the Mission Elapsed Time reference point.)
#
#REFERENCE:
#   (Trick Orbital Dynamics package Time documentation.)
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((Time is input in UTC.))
#
# 
#*/

def set_date_n_time_MET_start_16Jul1969(jeod_time) :

  #// MET initialization data.

  jeod_time.time_met.epoch_defined_in_name = "UTC"
  jeod_time.time_met.epoch_format = trick.TimeEnum.calendar
  jeod_time.time_met.epoch_year   = 1969
  jeod_time.time_met.epoch_month  = 7
  jeod_time.time_met.epoch_day    = 16
  jeod_time.time_met.epoch_hour   = 13
  jeod_time.time_met.epoch_minute = 32
  jeod_time.time_met.epoch_second =  0.0
