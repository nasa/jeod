#/*****************************************************************************
#     Run #8B: Rotational Propagation and State Comparison: Initial Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the numerical integration routines
#for the propagation of the rotational state of an orbital vehicle.
#The initial ISS rotational state is at Docking Torque Equilibrium Attitude
#with orbit angular rate.  (Orbit rate is approximately .065 deg/sec.)
#The initial inertial rotational state has a non zero orbital rate due to
#the above initial state.  Inertial attitude will not be constant, LVLH
#attitude will rotate in pitch at the orbital rate.
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
#      Mass Configuration:   sphere
#      Orbit configuration:  Typical
#
#   Case Setup -
#      Initial rotation rate = LVLH
#      External torques = No
#
#   Simulation Duration - 28800 sec (8 hrs)
#
#   Data Collection Rate - 60 sec.
#
#
#
#               Update to JEOD version 2.0
#
#
#*****************************************************************************/


exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# Reconfigure mass to be sphere
set_mass_sphere()
