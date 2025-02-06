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
  dr_group.add_variable( "jeod_time.gps.trunc_julian_time")
  dr_group.add_variable( "jeod_time.tai.trunc_julian_time")
  dr_group.add_variable( "jeod_time.tdb.trunc_julian_time")
  dr_group.add_variable( "jeod_time.tt.trunc_julian_time")
  dr_group.add_variable( "jeod_time.ut1.trunc_julian_time")
  dr_group.add_variable( "jeod_time.utc.trunc_julian_time")
  dr_group.add_variable( "jeod_time.gmst.seconds")
  dr_group.add_variable( "jeod_time.gps.seconds_of_day")
  dr_group.add_variable( "jeod_time.metveh1.clock_second")
  dr_group.add_variable( "jeod_time.metveh1.clock_minute")
  dr_group.add_variable( "jeod_time.metveh2.clock_second")
  dr_group.add_variable( "jeod_time.metveh2.clock_minute")
  dr_group.add_variable( "jeod_time.tai.calendar_second")
  dr_group.add_variable( "jeod_time.tdb.calendar_second")
  dr_group.add_variable( "jeod_time.tt.calendar_second")
  dr_group.add_variable( "jeod_time.ut1.calendar_second")
  dr_group.add_variable( "jeod_time.utc.calendar_second")
  dr_group.add_variable( "jeod_time.manager.dyn_time.seconds")

  trick.add_data_record_group(dr_group)
  return
