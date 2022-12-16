#/*****************************************************************************
#         Run #6C: Maneuver Burn Comparison: Plane Change Maneuver
#******************************************************************************
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
#      Mass Configuration:   cylinder
#      Orbit configuration:  Typical
#      Add'l notes:          Applied force on y-axis; initially aligned with LVLH
#
#
#
#
#*****************************************************************************/

exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# Set initial attitude off LVLH
vehicle.lvlh_init.orientation.euler_angles = [0.0, 0.0, 0.0]

# Reconfigure mass to be cylinder
set_mass_cylinder()

# Begin maneuver.
trick.add_read(1000.0, "vehicle.force_extern.force  = [ 0.0, -29000.0, 0.0]")
# End maneuver
trick.add_read(1048.0, "vehicle.force_extern.force  = [ 0.0, 0.0, 0.0]")
