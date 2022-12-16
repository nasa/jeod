#/*****************************************************************************
#         Run #9A: Rotational State Comparison: Torque, Zero Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the numerical integration routines and
#math modeling for the propagation of the rotational state of an orbital
#vehicle.  The initial ISS rotational state is at Docking Torque Equilibrium
#Attitude with zero angular rates in the inertial reference frame.  The rate
#reference frame is wrt to the inertial frame and is set to zero.  A small
#positive torque of 10 newton meters is applied in the roll angle direction
#wrt to the structural reference frame.  The torque is applied for 1000
#seconds starting at time step 1000 seconds into the run.
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
#      Add'l notes:          Attitude rate initialized wrt inertial
#                            External torque applied.
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


exec(compile(open( "SET_test/RUN_9B/input.py", "rb").read(), "SET_test/RUN_9B/input.py", 'exec'))

# Reconfigure attitude rate to be absed off inertial frame
set_rot_rate_inrtl()
