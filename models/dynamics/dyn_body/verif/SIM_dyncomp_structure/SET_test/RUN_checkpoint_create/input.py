#*****************************************************************************
#           Run checkpoint:  Test the checkpoint and restart mechanism
#*****************************************************************************/
# This file defines the scenario for checkpoint capture. It sets up 2 checkpoint files.
# The first checkpoint file is to be captured during the burn maneuver and the second to be capture sometime after the
# burn completes. The first checkpoint file is the intended file to be used for the restart RUN
#
#Description:
#The purpose of this test is to verify the checkpoint mechanism
#
#Run Scenario:
#   Model Setup -
#      Gravity Model: On
#         Earth:     8x8
#         Sun/Moon:  On
#         Gradient:  On
#      Gravity Gradient Torque: On
#      Atmosphere Model: On
#         F10.7 = 128.8
#         Geomagnetic Index = 15.7
#      Aerodynamic Drag Model: On
#         Coefficient of drag  = 2.0
#      Mass Configuration:   iss
#      Orbit configuration:  Elliptical
#      Add'l notes:          Applied force on y-axis; initially aligned with LVLH
#
#   Simulation Duration - 12000 sec (8 hrs)
#
#   Data Collection Rate - 10 sec., 600 sec.
#*****************************************************************************/

# Import the JEOD checkpoint/restart module.
import sys
import os
JEOD_HOME = os.getenv("JEOD_HOME")
if JEOD_HOME is None:
    JEOD_HOME = os.path.abspath(os.path.join(os.getcwd(),"../../../../.."))
sys.path.append ('/'.join([JEOD_HOME, "lib/jeod/python"]))
import jeod_checkpoint_restart

exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

# Add mass pt for attachment
add_mass_pt()

# put vehicle in elliptical orbit
set_trans_init_elliptical()

# Reconfigure gravity to 8x8
vehicle.earth_grav_control.spherical = False
vehicle.earth_grav_control.degree    = 8
vehicle.earth_grav_control.order     = 8
vehicle.earth_grav_control.gradient_degree = 8
vehicle.earth_grav_control.gradient_order  = 8

# Turn on gravity-torque
vehicle.grav_torque.active = True

# Turn on sun and moon
vehicle.sun_grav_control.active = True
vehicle.moon_grav_control.active = True

# Configure aerodynamics.
vehicle.aero_drag.active = True
set_aero_drag_iss()

# Begin maneuver.
trick.add_read(2000.0, """
vehicle.force_extern.force  = [ 0.0, -29000.0, 0.0]
""")
# End maneuver
trick.add_read(2050.0, """
vehicle.force_extern.force  = [ 0.0, 0.0, 0.0]
""")

exec(compile(open( "SET_test/chkpt_restart_times.py", "rb").read(), "SET_test/chkpt_restart_times.py", 'exec'))

trick.stop(stop_time)

## Take 2 checkpoints with hexfloat representation
# Drop a checkpoint. NOTE - this line must go after the stop setting
jeod_checkpoint_restart.create_checkpoint (chkpt_restart_time,
                                           stop_time)
trick.checkpoint(second_chkpt_time)

