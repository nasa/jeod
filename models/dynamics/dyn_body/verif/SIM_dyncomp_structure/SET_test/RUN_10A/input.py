#/*****************************************************************************
#   Run #10A: Rot State Comparison: Gravity Gradient, Circular, 0 Rate
#******************************************************************************
#
#Description:
#The purpose of this test is to verify the numerical integration routines and
#math model for the propagation of the rotational state of an orbital vehicle.
#The orbital vehicle is a special ISS 'cylinder-like' configuration with a mass
#of 1000kg, 12 meters long and 1 meter in radius.  The inertia matrix in the
#structural reference fram has center of mass at 6.0,0.0,0.0 meters.  The
#inertia matrix is diagonal with Ixx = 500.0 , Iyy = 12250.0 , Izz = 12250.0
#(kgM^2).  The initial ISS rotational state is at a special attitude.  Attitude
#is set wrt the LVLH frame.  In in-plane displacement (Pitch) is 5 degrees, the
#out-or-plane displacement is 1 degree.  The analytic solution for this
#'cylinder' is that the amplitude of the gravity gradient libration will be a
#max and min of 5 degrees in plane with a period of 3257.94 seconds and an out
#of plane libration max and min of 1 degrees with a period of 2821.46 seconds.
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
#      Orbit configuration:  Typical
#      Add'l notes:          High orientation
#
#   Simulation Duration - 28800 sec (8 hrs)
#
#   Data Collection Rate - 60 sec.
#
#
# / Update to JEOD version 2.0
#
#
#****************************************************************************


exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

#Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# Make the Earth gravity spherical
# and turn off the Sun and Moon gravity affects.
vehicle.earth_grav_control.spherical =True
vehicle.sun_grav_control.active = False
vehicle.moon_grav_control.active = False

# Turn on gravity-torque
vehicle.grav_torque.active = True

# Reconfigure vehicle mass as cylinder
set_mass_cylinder()

# Reconfigure initial attitude
vehicle.lvlh_init.orientation.euler_sequence      = trick.Orientation.Roll_Pitch_Yaw
vehicle.lvlh_init.orientation.euler_angles  = trick.attach_units( "degree",[ 0.0, 85.0, 1.0])
