#/*****************************************************************************
#            Run #6B: Aerodynamics Drag Comparison: Varying Density.
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
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: On
#         Coefficient of drag = 0.02
#      Mass Configuration:   sphere 
#      Orbit configuration:  Elliptical
#      Add'l notes:          Aero uses constant density
#
#   Simulation Duration - 28800 sec (8 hrs)
#
#   Data Collection Rate - 60 sec.
#
#
#
#
#*****************************************************************************/

exec(compile(open( "SET_test/RUN_6B/input.py", "rb").read(), "SET_test/RUN_6B/input.py", 'exec'))

# Set aero to constant density
set_aero_const_density_drag()
