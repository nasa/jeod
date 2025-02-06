################TRICK HEADER#######################################
#PURPOSE:
#  (To define the elements to be logged in the logging of rec) 
####################################################################################

def log_rec ( log_cycle ) :
  recording_group_name =  "time"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable( "jeod_time.time_tai.julian_date")
  dr_group.add_variable( "jeod_time.time_utc.julian_date")
  dr_group.add_variable( "jeod_time.time_tai.trunc_julian_time")
  dr_group.add_variable( "jeod_time.time_utc.trunc_julian_time")
  dr_group.add_variable( "jeod_time.time_ut1.trunc_julian_time")
  dr_group.add_variable( "jeod_time.time_manager.dyn_time.seconds")
  dr_group.add_variable( "jeod_time.time_tai.calendar_second")
  dr_group.add_variable( "jeod_time.time_ut1.calendar_second")
  dr_group.add_variable( "jeod_time.time_utc.calendar_second")

  trick.add_data_record_group(dr_group)
  return
