#/*****************************************************************************
#                     Run #3B: 8x8 Gravity Model Test
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the NASA simulation with the 8x8 gravity
#geopotential model.
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth: 8x8
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
#*****************************************************************************/

exec(compile(open( "SET_test/RUN_3A/input.py", "rb").read(), "SET_test/RUN_3A/input.py", 'exec'))

# Reconfigure gravity to 8x8:
vehicle.earth_grav_control.degree    = 8
vehicle.earth_grav_control.order     = 8
