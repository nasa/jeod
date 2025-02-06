#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for earth time data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (None)
#
#
#*/

def log_earth_time_rec ( log_cycle ) :
  recording_group_name =  "Earth_Time"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "jeod_time.conv_tai_ut1.a_to_b_offset")
  dr_group.add_variable(   "jeod_time.conv_tai_utc.a_to_b_offset")
  dr_group.add_variable(   "jeod_time.utc.trunc_julian_time")
  dr_group.add_variable(   "jeod_time.ut1.calendar_second")
  dr_group.add_variable(   "jeod_time.utc.calendar_second")
  dr_group.add_variable(   "jeod_time.tai.calendar_second")
  dr_group.add_variable(   "jeod_time.tt.calendar_second")
  dr_group.add_variable(   "jeod_time.manager.dyn_time.seconds")

  trick.add_data_record_group(dr_group)
  return
