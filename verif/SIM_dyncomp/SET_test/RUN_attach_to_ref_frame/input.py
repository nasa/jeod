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
import math
import os
JEOD_HOME = os.getenv("JEOD_HOME")
if JEOD_HOME is None:
    JEOD_HOME = os.path.abspath(os.path.join(os.getcwd(),"../../"))
sys.path.append ('/'.join([JEOD_HOME, "lib/jeod/python"]))
import jeod_checkpoint_restart

exec(compile(open( "SET_test/common_input.py", "rb").read(), "SET_test/common_input.py", 'exec'))
add_mass_pt()

# Set logging
LOG_CYCLE = 60.0
exec(compile(open( "Log_data/log_suite.py", "rb").read(), "Log_data/log_suite.py", 'exec'))

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

trick.add_read(1000.0, '''
pre_attach_vel = [0.0] * 3
for ii in range(3):
   pre_attach_vel[ii] = float(vehicle.dyn_body.composite_body.state.trans.velocity[ii])
vehicle.dyn_body.attach_to_frame("Earth.pfix")
''')

trick.add_read(1400.0, '''
vehicle.dyn_body.detach()
resume_state = trick.RefFrameState()
for ii in range(3):
   resume_state.trans.velocity[ii] = pre_attach_vel[ii]
vehicle.dyn_body.set_state(trick.RefFrameItems.Vel, resume_state, vehicle.dyn_body.composite_body)
''')

trick.add_read(1800.0, '''
pre_attach_vel = [0.0] * 3
for ii in range(3):
   pre_attach_vel[ii] = float(vehicle.dyn_body.composite_body.state.trans.velocity[ii])
vehicle.dyn_body.attach_to_frame(earth.planet.pfix)
''')

trick.add_read(2200.0, '''
vehicle.dyn_body.detach()
resume_state = trick.RefFrameState()
for ii in range(3):
   resume_state.trans.velocity[ii] = pre_attach_vel[ii]
vehicle.dyn_body.set_state(trick.RefFrameItems.Vel, resume_state, vehicle.dyn_body.composite_body)
''')

trick.add_read(2600.0, '''
# Store current inrtl state
pre_attach_pos = [0.0] * 3
pre_attach_vel = [0.0] * 3
pre_attach_rate = [0.0] * 3
pre_attach_rotation = [[0.0] * 3] * 3
for ii in range(3):
   pre_attach_pos[ii] = float(vehicle.dyn_body.composite_body.state.trans.position[ii])
   pre_attach_vel[ii] = float(vehicle.dyn_body.composite_body.state.trans.velocity[ii])
   pre_attach_rate[ii] = float(vehicle.dyn_body.composite_body.state.rot.ang_vel_this[ii])
   for jj in range(3):
      pre_attach_rotation[ii][jj] = float(vehicle.dyn_body.composite_body.state.rot.T_parent_this[ii][jj])
# Calc cart coords on "surface"
alt_scale = (earth.planet.r_eq + vehicle.pfix.state.sphere_coords.altitude-1.0) / earth.planet.r_eq
alt_scale = 1.0 / alt_scale
pfix_vec_orig = [0.0] * 3
for ii in range(3):
  pfix_vec_orig[ii] = vehicle.pfix.state.cart_coords[ii]
  attach_to_frame_helper.position[ii] = float(pfix_vec_orig[ii] * alt_scale)
  for jj in range(3):
     attach_to_frame_helper.rotation[ii][jj] = float(vehicle.dyn_body.composite_body.state.rot.T_parent_this[ii][jj])

pfix_vec_orig_mag = math.sqrt(pfix_vec_orig[0] * pfix_vec_orig[0] + pfix_vec_orig[1] * pfix_vec_orig[1] + pfix_vec_orig[2] * pfix_vec_orig[2])
pfix_vec_mag = math.sqrt(attach_to_frame_helper.position[0] * attach_to_frame_helper.position[0] + attach_to_frame_helper.position[1] * attach_to_frame_helper.position[1] + attach_to_frame_helper.position[2] * attach_to_frame_helper.position[2])
print('Original Vehicle pfix vector = {0}, magnitude = {1}'.format(pfix_vec_orig, pfix_vec_orig_mag))
print('Target New Vehicle pfix vector = {0}, magnitude = {1}'.format(attach_to_frame_helper.position, pfix_vec_mag))

# Turn off atmosphere to avoid -sqrt somewhere
trick.exec_set_job_onoff("vehicle.atmos_state.update_state", 2, False)

attach_to_frame_helper.attach_wrap_child_parent_pos_rot("test_point", "Earth.pfix")
''')

trick.add_read(3000.0, '''
pfix_vec = [0.0] * 3
for ii in range(3):
  pfix_vec[ii] = float(vehicle.pfix.state.cart_coords[ii])
pfix_vec_mag = math.sqrt(pfix_vec[0] * pfix_vec[0] + pfix_vec[1] * pfix_vec[1] + pfix_vec[2] * pfix_vec[2])
print('Actual New Vehicle pfix vector = {0}, magnitude = {1}'.format(pfix_vec, pfix_vec_mag))

vehicle.dyn_body.detach()
resume_state = trick.RefFrameState()
for ii in range(3):
   resume_state.trans.position[ii] = pre_attach_pos[ii]
   resume_state.trans.velocity[ii] = pre_attach_vel[ii]
   resume_state.rot.ang_vel_this[ii] = pre_attach_rate[ii]
   for jj in range(3):
      vehicle.dyn_body.composite_body.state.rot.T_parent_this[ii][jj] = pre_attach_rotation[ii][jj]
vehicle.dyn_body.set_state(trick.RefFrameItems.Pos_Vel_Att_Rate, resume_state, vehicle.dyn_body.composite_body)
trick.exec_set_job_onoff("vehicle.atmos_state.update_state", 2, True)
''')

trick.add_read(3400.0, '''
# Store current inrtl state
pre_attach_pos = [0.0] * 3
pre_attach_vel = [0.0] * 3
pre_attach_rate = [0.0] * 3
pre_attach_rotation = [[0.0] * 3] * 3
for ii in range(3):
   pre_attach_pos[ii] = float(vehicle.dyn_body.composite_body.state.trans.position[ii])
   pre_attach_vel[ii] = float(vehicle.dyn_body.composite_body.state.trans.velocity[ii])
   pre_attach_rate[ii] = float(vehicle.dyn_body.composite_body.state.rot.ang_vel_this[ii])
   for jj in range(3):
      pre_attach_rotation[ii][jj] = float(vehicle.dyn_body.composite_body.state.rot.T_parent_this[ii][jj])
# Calc cart coords on "surface"
alt_scale = (earth.planet.r_eq + vehicle.pfix.state.sphere_coords.altitude-1.0) / earth.planet.r_eq
alt_scale = 1.0 / alt_scale
pfix_vec_orig = [0.0] * 3
for ii in range(3):
  pfix_vec_orig[ii] = vehicle.pfix.state.cart_coords[ii]
  attach_to_frame_helper.position[ii] = float(pfix_vec_orig[ii] * alt_scale)
  for jj in range(3):
     attach_to_frame_helper.rotation[ii][jj] = float(vehicle.dyn_body.composite_body.state.rot.T_parent_this[ii][jj])

pfix_vec_orig_mag = math.sqrt(pfix_vec_orig[0] * pfix_vec_orig[0] + pfix_vec_orig[1] * pfix_vec_orig[1] + pfix_vec_orig[2] * pfix_vec_orig[2])
pfix_vec_mag = math.sqrt(attach_to_frame_helper.position[0] * attach_to_frame_helper.position[0] + attach_to_frame_helper.position[1] * attach_to_frame_helper.position[1] + attach_to_frame_helper.position[2] * attach_to_frame_helper.position[2])
print('Original Vehicle pfix vector = {0}, magnitude = {1}'.format(pfix_vec_orig, pfix_vec_orig_mag))
print('Target New Vehicle pfix vector = {0}, magnitude = {1}'.format(attach_to_frame_helper.position, pfix_vec_mag))

# Turn off atmosphere to avoid -sqrt somewhere
trick.exec_set_job_onoff("vehicle.atmos_state.update_state", 2, False)

attach_to_frame_helper.attach_wrap_pos_rot_parent(earth.planet.pfix)
''')

trick.add_read(3800.0, '''
pfix_vec = [0.0] * 3
for ii in range(3):
  pfix_vec[ii] = float(vehicle.pfix.state.cart_coords[ii])
pfix_vec_mag = math.sqrt(pfix_vec[0] * pfix_vec[0] + pfix_vec[1] * pfix_vec[1] + pfix_vec[2] * pfix_vec[2])
print('Actual New Vehicle pfix vector = {0}, magnitude = {1}'.format(pfix_vec, pfix_vec_mag))

vehicle.dyn_body.detach()
resume_state = trick.RefFrameState()
for ii in range(3):
   resume_state.trans.position[ii] = pre_attach_pos[ii]
   resume_state.trans.velocity[ii] = pre_attach_vel[ii]
   resume_state.rot.ang_vel_this[ii] = pre_attach_rate[ii]
   for jj in range(3):
      vehicle.dyn_body.composite_body.state.rot.T_parent_this[ii][jj] = pre_attach_rotation[ii][jj]
vehicle.dyn_body.set_state(trick.RefFrameItems.Pos_Vel_Att_Rate, resume_state, vehicle.dyn_body.composite_body)
trick.exec_set_job_onoff("vehicle.atmos_state.update_state", 2, True)
''')


# Begin maneuver.
trick.add_read(2000.0, "vehicle.force_extern.force  = [ 0.0, -29000.0, 0.0]")
# End maneuver
trick.add_read(2050.0, "vehicle.force_extern.force  = [ 0.0, 0.0, 0.0]")

exec(compile(open( "SET_test/chkpt_restart_times.py", "rb").read(), "SET_test/chkpt_restart_times.py", 'exec'))

trick.stop(stop_time)

## Take 2 checkpoints with hexfloat representation
# Drop a checkpoint. NOTE - this line must go after the stop setting
jeod_checkpoint_restart.create_checkpoint (chkpt_restart_time,
                                           stop_time)
trick.checkpoint(second_chkpt_time)

