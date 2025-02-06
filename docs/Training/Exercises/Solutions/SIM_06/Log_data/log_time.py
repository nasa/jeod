################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of rec) 
####################################################################################

def log_time ( log_cycle ) :
  recording_group_name =  "state"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  dr_group.add_variable( "jeod_time.time_utc.seconds")
  dr_group.add_variable( "jeod_time.time_utc.calendar_second")
  dr_group.add_variable( "jeod_time.time_utc.calendar_minute")
  dr_group.add_variable( "jeod_time.time_utc.calendar_hour")

  trick.add_data_record_group(dr_group)

  return
