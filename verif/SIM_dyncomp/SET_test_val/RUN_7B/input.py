#/*****************************************************************************
#            Run #7B: Full Translational Comparison: No Aero: 8x8
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that the output of the NASA simulation
#MET atmosphere model.
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth:     8x8
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

exec(compile(open( "SET_test/RUN_7A/input.py", "rb").read(), "SET_test/RUN_7A/input.py", 'exec'))

# Reconfigure Earth gravity to 8x8
vehicle.earth_grav_control.degree = 8
vehicle.earth_grav_control.order  = 8
