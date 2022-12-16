#*****************************************************************************
#           Run checkpoint:  Test the checkpoint and restart mechanism
#*****************************************************************************/
# This file defines the common input aspects shared between the run that
# creates the checkpoint (RUN_checkpoint_create) and the run that restarts from
# the checkpoint (RUN_checkpoint_restart)
#
#Description:
#The purpose of this test is to verify thei checkpoint mechanism
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
#
#   Simulation Duration - 12000 sec (8 hrs)
#
#   Data Collection Rate - 10 sec., 600 sec.
#*****************************************************************************/

# Import the JEOD checkpoint/restart module.
import sys 
import os
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
import jeod_checkpoint_restart

exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))

stop_time = 12000

# Log data
exec(compile(open( "Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state_ascii(600)
data_post_checkpoint = log_state_ascii_compare(10, "post_checkpoint")
data_full = log_state_ascii_compare(100, "full")
checkpoint_time = 600.0
comparison_window = 100.0 #either side
trick.dr_disable_group("post_checkpoint")
trick.dr_disable_group("full")

trick.add_read (checkpoint_time, """
trick.dr_enable_group("post_checkpoint")
trick.dr_enable_group("full")
""")
trick.add_read (checkpoint_time + comparison_window, """
trick.dr_disable_group("post_checkpoint")
""")

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

trick.stop(stop_time)
