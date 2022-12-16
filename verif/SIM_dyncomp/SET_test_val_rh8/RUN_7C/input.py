#/*****************************************************************************
#            Run #7C: Full Translational Comparison: With Aero: 4x4
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
#      Aerodynamic Drag Model: On
#         Coefficient of drag = 0.02
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
# Turn on aerodynamics
vehicle.aero_drag.active   = True
set_aero_drag_ballistic()
