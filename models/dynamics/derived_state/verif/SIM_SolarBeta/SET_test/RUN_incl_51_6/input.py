exec(compile(open( "SET_test/RUN_incl_51_6_sphericgrav/input.py", "rb").read(), "SET_test/RUN_incl_51_6_sphericgrav/input.py", 'exec'))
veh.grav_control.spherical = False
veh.grav_control_moon.active = True
veh.grav_control_sun.active  = True
