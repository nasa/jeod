#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for time data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (none)
#
# 
#*/

def log_time ( log_cycle ) :
  recording_group_name =  "time_values"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "jeod_time.manager.dyn_time.seconds")
  dr_group.add_variable(   "jeod_time.tai.seconds")
  dr_group.add_variable(   "jeod_time.utc.seconds")
  dr_group.add_variable(   "jeod_time.ut1.seconds")
  dr_group.add_variable(   "jeod_time.gmst.seconds")

  trick.add_data_record_group(dr_group)
  return
