#/*****************************************************************************
#    Run #9D: Rotational State Comparison: Torque and Force, Initial Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that numerical integration routines and
#math modeling for the propagation of the rotational state of an orbital
#vehicle.  The initial ISS rotational state is at Docking Torque Equilibrium
#Attitude with orbit angular rate.  (Orbit rate is approximately .065 deg/sec.)
#The initial inertial rotational state has a non zero orbital rate due to the
#above initial state.  A small positive torque of 10 newton meters is applied
#in the roll angle direction in the structural reference frame.  A small
#positive force of  10 newton meters is applied in the +X direction in the
#structural reference frame, for 1000 seconds starting at time step 1000
#seconds into the run.
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
#      Add'l notes:          External force and torque applied.
#
#   Simulation Duration - 28800 sec (8 hrs)
#
#   Data Collection Rate - 60 sec.
#
#
#
#               Update to JEOD version 2.0
#
#*****************************************************************************/

exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))


#/* Apply a constant force and torque. */
trick.add_read(1000.0, """
vehicle.force_extern.force   = [ 10.0, 0.0, 0.0]
vehicle.torque_extern.torque = [ 10.0, 0.0, 0.0]
""")
#/* Remove force and torque. */
trick.add_read(2000.0, """
vehicle.force_extern.force   = [ 0.0, 0.0, 0.0]
vehicle.torque_extern.torque = [ 0.0, 0.0, 0.0]
""")
