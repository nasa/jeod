#/*****************************************************************************
#      Run #9C: Rotational State Comparison: Torque and Force, Zero Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify that numerical integration routines
#and math modeling for the propagation of the rotational state of an orbital
#vehicle.  The initial ISS rotational state is at Docking Torque Equilibrium
#Attitude with an angular rotation set to orbit rate.  The initial rotational
#rate state is set to zero wrt the inertial frame.  A small positive torque of
#10 newton meters is applied in the roll angle direction in the structural
#reference frame.  A small positive force of 10 newton meters is applied in
#the +X direction in the structural reference frame, for 1000 seconds starting
#at time step 1000 seconds into the run
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
#                            External force and torque applied.
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


exec(compile(open( "SET_test/RUN_9D/input.py", "rb").read(), "SET_test/RUN_9D/input.py", 'exec'))

# Reconfigure attitude rate to be absed off inertial frame
set_rot_rate_inrtl()
