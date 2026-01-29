#/*****************************************************************************
#   Run #10D: Rot State Comparison: Gravity Gradient, Elliptic, Initial Rate
#******************************************************************************
#
#Description:
# This test is the same as 10C except that there is an initial attitude rate
# of 0.01 deg/s on the LVLH pitch axis with this one; this is the same cahnge
# that was applied to generate 10B from 10A.
#The purpose of this test is to verify the numerical integration routines and
#math model for the propagation of the rotational state of an orbital vehicle. 
#The orbital vehicle is a special ISS 'cylinder-like' configuration with a mass
#of 1000kg, 12 meters long and 1 meter in radius.  The inertia matrix in the
#structural reference fram has center of mass at 6.0,0.0,0.0 meters.  The
#inertia matrix is diagonal with Ixx = 500.0 , Iyy = 12250.0 , Izz = 12250.0
#(kgM^2).  The initial ISS rotational state is at a special attitude.  Attitude
#is set wrt the LVLH frame.  In in-plane displacement (Pitch) is 5 degrees, the
#out-or-plane displacement is 1 degree.  A rate in pitch is given in the LVLH
#frame of .01 deg/sec.  The analytic solution for this 'cylinder' is that the
#amplitude of the gravity gradient libration will be a max and min of 5 degrees
#in plane with a period of 3257.94 seconds and an out of plane libration max
#and min of 1 degrees with a period of 2821.46 seconds, with out an initial
#rate.  The initial pitch rate will give a small increase the pitch libration
#amplitude and phase shift in the libration period.
#
#References:
#   Spacecraft Attitude Dynamics, Peter C. Hughes, Dover Edition 2004,
#   pages (232 - 353)
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth:     Spherical
#         Sun/Moon:  Off
#         Gradient:  On
#      Gravity Gradient Torque: On
#      Atmosphere Model: On
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: Off
#      Mass Configuration:   Cylinder
#      Orbit configuration:  Elliptical
#      Add'l notes:          High orientation, small additional rate
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


exec(compile(open( "SET_test/RUN_10C/input.py", "rb").read(), "SET_test/RUN_10C/input.py", 'exec'))
vehicle.lvlh_init.ang_velocity = trick.attach_units( "degree/s",[ 0.0, 0.01, 0.0])
