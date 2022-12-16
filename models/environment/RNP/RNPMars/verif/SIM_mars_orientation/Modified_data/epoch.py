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

def set_Time_epoch(jeod_time) :

  #// Initialize from UTC calendar date.
  jeod_time.manager_init.initializer = "UTC"
  jeod_time.manager_init.sim_start_format = trick.TimeEnum.calendar

  #// Time initialization data.
  #//
  jeod_time.utc.calendar_year   = 2010
  jeod_time.utc.calendar_month  =   9
  jeod_time.utc.calendar_day    =   10
  jeod_time.utc.calendar_hour   =    0
  jeod_time.utc.calendar_minute =    0
  jeod_time.utc.calendar_second =    0.0

#  jeod_time.utc.calendar_year   = 2000
#  jeod_time.utc.calendar_month  =   1
#  jeod_time.utc.calendar_day    =   1
#  jeod_time.utc.calendar_hour   =    11
#  jeod_time.utc.calendar_minute =    58
#  jeod_time.utc.calendar_second =    55.8160000119824

#  jeod_time.utc.calendar_year   = 2005
#  jeod_time.utc.calendar_month  =   1
#  jeod_time.utc.calendar_day    =   1
#  jeod_time.utc.calendar_hour   =    0
#  jeod_time.utc.calendar_minute =    0
#  jeod_time.utc.calendar_second =    0

  jeod_time.tai.initialize_from_name = "UTC"
  jeod_time.ut1.initialize_from_name = "TAI"
  jeod_time.tt.initialize_from_name  = "TAI"
  jeod_time.gmst.initialize_from_name  = "UT1"

  jeod_time.tai.update_from_name = "Dyn"
  jeod_time.ut1.update_from_name = "TAI"
  jeod_time.utc.update_from_name = "TAI"
  jeod_time.tt.update_from_name  = "TAI"
  jeod_time.gmst.update_from_name  = "UT1"


  return
