trick.sim_services.exec_set_trap_sigfpe(1)

exec(compile(open("Log_data/log_met.py", "rb").read(), "Log_data/log_met.py", 'exec'))
dr_group = log_atmos_verif()

# cache the method for attaching units
set_units = trick.sim_services.attach_units

jeod_time.time_manager_init.initializer = "UTC"
jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar
# Configure base date
jeod_time.time_utc.calendar_year  = 2000
jeod_time.time_utc.calendar_month = 1
jeod_time.time_utc.calendar_day   = 1
jeod_time.time_utc.calendar_hour  = 1
jeod_time.time_utc.calendar_minute   = 31
jeod_time.time_utc.calendar_second = 48.0
jeod_time.time_utc.true_utc = False
jeod_time.time_utc.update_from_name = "TAI"

jeod_time.time_tai.update_from_name = "Dyn"
jeod_time.time_tai.initialize_from_name = "UTC"

planet.atmos.geo_index_type = trick.METAtmosphere.ATMOS_MET_GI_AP
planet.atmos.geo_index = 20.30
planet.atmos.F10 = 230.00
planet.atmos.F10B = 230.00

vehicle.pos.ellip_coords.altitude = set_units("km", 100.00)
vehicle.pos.ellip_coords.latitude = set_units("degree", 55.0)
vehicle.pos.ellip_coords.longitude = set_units("degree", 45.0)

# Sim is testing the atmosphere at the same time over and over at
# different positions, so turn off the time sim-object.  After
# initialization, it is no longer needed.
trick.add_read(0, """
trick.exec_set_sim_object_onoff('jeod_time', 0)
""")
