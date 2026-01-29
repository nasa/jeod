#/*****************************************************************************
#        Run #9B: Rotational State Comparison: Torque, Initial Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that numerical integration routines and
#math model for the propagation of the rotational state of an orbital vehicle.
#The initial ISS rotational state is at Docking Torque Equilibrium Attitude
#with orbit angular rate.  (Orbit rate is approximately .065 deg/sec.) The
#initial inertial rotational state has a non zero orbital rate due to the above
#initial state.  Inertial attitude will not be constant, LVLH attitude will
#rotate in pitch at the orbital rate.  For 1000 seconds starting at time step
#1000 seconds into the run.
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
#      Add'l notes:          External torque applied.
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

# Apply a constant torque.
trick.add_read(1000.0, "vehicle.torque_extern.torque  = [ 10.0, 0.0, 0.0]")
# Remove torque.
trick.add_read(2000.0, "vehicle.torque_extern.torque  = [  0.0, 0.0, 0.0]")
