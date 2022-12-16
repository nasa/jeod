exec(compile(open("Log_data/log_test.py", "rb").read(), "Log_data/log_test.py", 'exec'))
earth.inrtl_pos = [1,1,1]
earth.altitude = 170000
earth.wind_velocity.omega = 1

#trick.add_read( 1.0, "earth.altitude = 170000+(1*10000)")
#trick.add_read( 2.0, "earth.altitude = 170000+(2*10000)")
for ii in range(1,25):
  trick.add_read(ii*1.0, "earth.altitude = 170000+{0}*10000.0".format(ii))

trick.stop(24)
