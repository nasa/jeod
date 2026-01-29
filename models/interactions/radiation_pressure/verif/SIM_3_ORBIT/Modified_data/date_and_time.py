################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_date_and_time
####################################################################################
jeod_time.time_manager_init.initializer = "TAI"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

jeod_time.time_tai.set_date_and_time(1998, 12, 1, 0, 0, 31.0)  # i.e. UTC = 0:00:00

jeod_time.time_tai.update_from_name = "Dyn"

jeod_time.time_tt.initialize_from_name = "TAI"
jeod_time.time_tt.update_from_name = "TAI"
