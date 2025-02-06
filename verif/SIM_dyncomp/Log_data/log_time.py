def log_add_utc_calendar( dr_group) : 
  dr_group.add_variable(   "jeod_time.time_utc.calendar_year")
  dr_group.add_variable(   "jeod_time.time_utc.calendar_month")
  dr_group.add_variable(   "jeod_time.time_utc.calendar_day")
  dr_group.add_variable(   "jeod_time.time_utc.calendar_hour")
  dr_group.add_variable(   "jeod_time.time_utc.calendar_minute")
  dr_group.add_variable(   "jeod_time.time_utc.calendar_second")


def log_time ( log_cycle ) :
  recording_group_name =  "Earth_Time"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "jeod_time.time_converter_tai_ut1.a_to_b_offset")
  dr_group.add_variable(   "jeod_time.time_converter_tai_utc.a_to_b_offset")
  dr_group.add_variable(   "jeod_time.time_utc.trunc_julian_time")
  dr_group.add_variable(   "jeod_time.time_ut1.calendar_second")
  dr_group.add_variable(   "jeod_time.time_utc.calendar_second")
  dr_group.add_variable(   "jeod_time.time_tai.calendar_second")
  dr_group.add_variable(   "jeod_time.time_tt.calendar_second")
  dr_group.add_variable(   "jeod_time.time_manager.dyn_time.seconds")

  trick.add_data_record_group(dr_group)
