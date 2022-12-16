#/*****************************************************************************
#       Run #8A: Rotational Propagation and State Comparison: Zero Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the numerical integration routines and
#math model for the propagation of the rotational state of an orbital vehicle.
#The initial ISS rotational state is at Docking Torque Equilibrium Attitude
#with zero angular rates.  Attitude is set wrt inertial frame.  Inertial
#attitude should be constant, LVLH attitude will rotate in pitch.
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
#      Add'l notes:          Attitude rate initialized wrt inertial
#
#         Earth = Spherical
#         Planetary ephemeris = Off
#         Third body perturbations = Off
#      Gravity Gradient Torque: Off
#      Atmosphere Model: On
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: Off
#         Coefficient of drag = N/A
#
#   Case Setup -
#      Initial rotation rate = No
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

exec(compile(open( "SET_test/RUN_8B/input.py", "rb").read(), "SET_test/RUN_8B/input.py", 'exec'))

# Reconfigure attitude rate to be absed off inertial frame
set_rot_rate_inrtl()
