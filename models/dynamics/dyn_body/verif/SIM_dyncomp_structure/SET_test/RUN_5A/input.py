#/*****************************************************************************
#            Run #5A: Atmosphere Comparison: Minimum Solar Activity
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that the output of the NASA simulation
#MET atmosphere model.
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth: Spherical
#         Sun/Moon:  Off
#         Gradient:  On
#      Gravity Gradient Torque: Off
#      Atmosphere Model: On
#         F10.7 = 70.0
#         Geomagnetic Index = 0.0
#      Aerodynamic Drag Model: Off
#      Mass Configuration:   iss
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


exec(compile(open( "SET_test/RUN_5B/input.py", "rb").read(), "SET_test/RUN_5B/input.py", 'exec'))

# Reconfigure the atmosphere to low solar flux
set_solar_flux("min")
