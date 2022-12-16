exec(compile(open("SET_test/input_core.py", "rb").read(), "SET_test/input_core.py", 'exec'))

trick.add_read (2 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 150.00)
""")

trick.add_read (3 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 200.00)
""")

trick.add_read (4 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 250.00)
""")

trick.add_read (5 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 300.00)
""")

trick.add_read (6 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 350.00)
""")

trick.add_read (7 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 400.00)
""")

trick.add_read (8 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 450.00)
""")

trick.add_read (9 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 500.00)
""")

trick.add_read (10 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 550.00)
""")

trick.add_read (11 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 600.00)
""")

trick.add_read (12 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 650.00)
""")

trick.add_read (13 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 700.00)
""")

trick.add_read (14 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 750.00)
""")

trick.add_read (15 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 800.00)
""")

trick.add_read (16 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 850.00)
""")

trick.add_read (17 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 900.00)
""")

trick.add_read (18 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 950.00)
""")

trick.add_read (19 , """
vehicle.pos.ellip_coords.altitude = set_units("km", 1000.00)
""")

trick.exec_set_terminate_time(20)

