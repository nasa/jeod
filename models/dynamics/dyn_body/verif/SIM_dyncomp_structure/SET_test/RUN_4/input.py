#/*****************************************************************************
#                    Run #4: Planetary Ephemeris Comparison
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that the output of the NASA simulation
#planetary ephemeris model.  (The atmosphere model uses the geocentric position
#of the Sun.  The 3rd body perturbation model uses the geocentric position of
#the Sun and Moon.)
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth: Spherical
#         Sun/Moon:  On
#         Gradient:  On
#      Gravity Gradient Torque: Off
#      Atmosphere Model: On
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: Off
#      Mass Configuration:   iss
#      Orbit configuration:  Typical
#
#   Simulation Duration - 28800 sec (8 hrs)
#
#   Data Collection Rate - 60 sec.
#
#
#
#*****************************************************************************/


exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# Turn on sun and moon gravity
vehicle.sun_grav_control.active = True
vehicle.moon_grav_control.active = True
