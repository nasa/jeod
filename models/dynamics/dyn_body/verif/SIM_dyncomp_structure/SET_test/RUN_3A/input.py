#/*****************************************************************************
#                     Run #3A: 4x4 Gravity Model Test
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the NASA simulation with the 4x4 gravity
#geopotential model.
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth: 4x4
#         Sun/Moon:  Off
#         Gradient:  Off
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
#
#*****************************************************************************/


exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# Reconfigure gravity to 4x4
vehicle.earth_grav_control.spherical = False
vehicle.earth_grav_control.degree = 4
vehicle.earth_grav_control.order  = 4

# Turn off grav gradient
vehicle.earth_grav_control.gradient  = False
