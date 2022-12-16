#/*****************************************************************************
#                      Run #1: Simulation Constants
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the numerical constants associated with
#the orbital dynamics models used in the simulation.
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
#      Aerodynamic Drag Model: Off
#      Mass Configuration:   iss
#      Orbit configuration:  Typical
#      Add'l notes:          Short run, logging constants only
#
#   Simulation Duration - 1 sec
#
#   Data Collection Rate - 1 sec.
#
#*****************************************************************************/


exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
exec(compile(open( "Log_data/log_constants.py", "rb").read(), "Log_data/log_constants.py", 'exec'))
log_constants( 1.0 )

trick.stop(1.0)
