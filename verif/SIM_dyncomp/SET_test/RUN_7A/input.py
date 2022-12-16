#/*****************************************************************************
#            Run #7A: Full Translational Comparison: No Aero: 4x4
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that the output of the NASA simulation
#MET atmosphere model.
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth:     4x4
#         Sun/Moon:  On
#         Gradient:  Off
#      Gravity Gradient Torque: Off
#      Atmosphere Model: On
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: Off
#      Mass Configuration:   sphere
#      Orbit configuration:  Elliptical
#
#   Simulation Duration - 28800 sec (8 hrs)
#
#   Data Collection Rate - 60 sec.
#
#
#
#
#*****************************************************************************/

exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# put vehicle in elliptical orbit
set_trans_init_elliptical()

# Reconfigure gravity to 4x4
vehicle.earth_grav_control.spherical = False
vehicle.earth_grav_control.degree = 4
vehicle.earth_grav_control.order  = 4

# Turn off gravity gradient
vehicle.earth_grav_control.gradient = False

# Turn on sun and moon gravity
vehicle.sun_grav_control.active = True
vehicle.moon_grav_control.active = True

# Reconfigure mass to be cylinder
set_mass_sphere()
