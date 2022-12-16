exec(compile(open("SET_test/input_core.py", "rb").read(), "SET_test/input_core.py", 'exec'))
log_add_met(dr_group)

jeod_time.time_utc.calendar_year  = 1995
jeod_time.time_utc.calendar_hour  = 0
jeod_time.time_utc.calendar_minute   = 0
jeod_time.time_utc.calendar_second = 1.0

vehicle.pos.ellip_coords.altitude = set_units("km", 140.00)
vehicle.pos.ellip_coords.latitude = set_units("degree", 28.45)
vehicle.pos.ellip_coords.longitude = set_units("degree", -80.530)

trick.add_read(2 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 138.00)
""")

trick.add_read(3 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 136.00)
""")

trick.add_read(4 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 134.00)
""")

trick.add_read(5 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 132.00)
""")

trick.add_read(6 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 130.00)
""")

trick.add_read(7 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 128.00)
""")

trick.add_read(8 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 126.00)
""")

trick.add_read(9 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 124.00)
""")

trick.add_read(10 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 122.00)
""")

trick.add_read(11 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 120.00)
""")

trick.exec_set_terminate_time(12)
