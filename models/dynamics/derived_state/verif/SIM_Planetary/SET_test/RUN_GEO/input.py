exec(compile(open("input_common.py", "rb").read(), "input_common.py", 'exec'))
# Make orbit approx geosynchronous
veh.init_orbit.alt_apoapsis  = trick.attach_units( "km",35786.0)
veh.init_orbit.alt_periapsis  = trick.attach_units( "km",35786.0)
