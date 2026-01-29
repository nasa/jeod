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

def set_date_n_time_epoch(jeod_time) :

  jeod_time.tai.update_from_name = "Dyn"

  jeod_time.manager_init.initializer = "TAI"
  jeod_time.manager_init.sim_start_format = trick.TimeEnum.calendar

  jeod_time.tai.set_date_and_time(2005, 12, 31, 23, 59, 50.0)

  return
