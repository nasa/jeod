exec(compile(open("SET_test/input_core.py", "rb").read(), "SET_test/input_core.py", 'exec'))

trick.add_read(2 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 120.00)
""")

trick.add_read(3 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 140.00)
""")

trick.add_read(4 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 160.00)
""")

trick.add_read(5 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 180.00)
""")

trick.add_read(6 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 200.00)
""")

trick.add_read(7 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 220.00)
""")

trick.add_read(8 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 240.00)
""")

trick.add_read(9 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 260.00)
""")

trick.add_read(10 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 280.00)
""")

trick.add_read(11 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 300.00)
""")

trick.add_read(12 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 320.00)
""")

trick.add_read(13 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 340.00)
""")

trick.add_read(14 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 360.00)
""")

trick.add_read(15 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 380.00)
""")

trick.add_read(16 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 400.00)
""")

trick.add_read(17 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 420.00)
""")

trick.add_read(18 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 440.00)
""")

trick.add_read(19 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 460.00)
""")

trick.add_read(20 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 480.00)
""")

trick.add_read(21 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 500.00)
""")

trick.exec_set_terminate_time(22)
