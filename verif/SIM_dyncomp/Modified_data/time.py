#// Initialize from UTC calendar date.
jeod_time.time_manager_init.initializer = "UTC"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

#// Time initialization data.
#// Midnight on November 20, 2007.
jeod_time.time_utc.calendar_year   = 2007
jeod_time.time_utc.calendar_month  =   11
jeod_time.time_utc.calendar_day    =   20
jeod_time.time_utc.calendar_hour   =    0
jeod_time.time_utc.calendar_minute =    0
jeod_time.time_utc.calendar_second =  0.0

jeod_time.time_tai.initialize_from_name = "UTC"
jeod_time.time_ut1.initialize_from_name = "TAI"
jeod_time.time_tt.initialize_from_name  = "TAI"
jeod_time.time_gmst.initialize_from_name  = "UT1"

jeod_time.time_tai.update_from_name = "Dyn"
jeod_time.time_ut1.update_from_name = "TAI"
jeod_time.time_utc.update_from_name = "TAI"
jeod_time.time_tt.update_from_name  = "TAI"
jeod_time.time_gmst.update_from_name = "UT1"

jeod_time.time_utc.true_utc = False
jeod_time.time_ut1.true_ut1 = False

#// Override the time computation parameters.
jeod_time.time_converter_tai_utc.override_data_table = True
jeod_time.time_converter_tai_utc.leap_sec_override_val = 32

jeod_time.time_converter_tai_ut1.override_data_table = True
jeod_time.time_converter_tai_ut1.tai_to_ut1_override_val = -32.469
