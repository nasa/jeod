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
sys.path.append ('/'.join([os.getenv("JEOD_HOME"), "lib/jeod/python"]))
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

## Allocate a DynBody and attach to the vehicle to demonstrate some chkpt/restart
child_vehicle = trick.castAsDynBodySimObject(trick.TMM_declare_var_s("DynBodySimObject child_vehicle"))
trick.exec_add_sim_object(child_vehicle, "child_vehicle")
child_vehicle.dyn_manager_ptr = dynamics.dyn_manager
child_vehicle.dyn_body.integ_frame_name = "Earth.inertial"
child_vehicle.dyn_body.translational_dynamics = True
child_vehicle.dyn_body.rotational_dynamics = True

child_earth_grav_control = trick.castAsjeod__SphericalHarmonicsGravityControls(trick.TMM_declare_var_s("jeod::SphericalHarmonicsGravityControls child_earth_grav_control"))
child_moon_grav_control  = trick.castAsjeod__SphericalHarmonicsGravityControls(trick.TMM_declare_var_s("jeod::SphericalHarmonicsGravityControls child_moon_grav_control"))
child_sun_grav_control   = trick.castAsjeod__SphericalHarmonicsGravityControls(trick.TMM_declare_var_s("jeod::SphericalHarmonicsGravityControls child_sun_grav_control"))

# Set up the gravity controls for the Earth.
child_earth_grav_control.source_name     = "Earth"
child_earth_grav_control.active          = True
child_earth_grav_control.spherical       = True
child_earth_grav_control.gradient        = True

# Set up the gravity controls for the Moon.
child_moon_grav_control.source_name = "Moon"
child_moon_grav_control.active      = False
child_moon_grav_control.spherical   = True
child_moon_grav_control.gradient    = True

# Set up the gravity controls for the Sun.
child_sun_grav_control.source_name = "Sun"
child_sun_grav_control.active      = False
child_sun_grav_control.spherical   = True
child_sun_grav_control.gradient    = True


child_vehicle.dyn_body.grav_interaction.add_control(child_earth_grav_control)
child_vehicle.dyn_body.grav_interaction.add_control(child_moon_grav_control)
child_vehicle.dyn_body.grav_interaction.add_control(child_sun_grav_control)

child_vehicle.dyn_body.set_name("child_vehicle")

# Set the child_vehicle_mass porperties for this child_vehicle_
child_vehicle_mass_init = trick.castAsjeod__MassBodyInit(trick.TMM_declare_var_s("jeod::MassBodyInit child_vehicle_mass_init"))
child_vehicle_mass_init.set_subject_body( child_vehicle.dyn_body.mass )

# Set the struct to body reference frame orientation.
child_vehicle_mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
child_vehicle_mass_init.properties.pt_orientation.eigen_angle = 0.0
child_vehicle_mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]

child_vehicle_mass_init.allocate_points(1)

child_vehicle_mass_init.get_mass_point(0).set_name('child_test_point')
child_vehicle_mass_init.get_mass_point(0).pt_orientation.data_source = trick.Orientation.InputEulerRotation
child_vehicle_mass_init.get_mass_point(0).pt_orientation.euler_sequence = trick.Orientation.Roll_Pitch_Yaw

# Set the struct to body reference frame orientation.
child_vehicle_mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
child_vehicle_mass_init.properties.pt_orientation.eigen_angle = 0.0
child_vehicle_mass_init.properties.pt_orientation.eigen_axis  = [ 0.0, 1.0, 0.0]

# Models a sphere with radius = 1.0 m and mass = 1.0 kg
child_vehicle_mass_init.properties.mass        = 1.0
child_vehicle_mass_init.properties.position    = [ 0.0, 0.0, 0.0]
child_vehicle_mass_init.properties.inertia[0]  = [ 0.4, 0.0, 0.0]
child_vehicle_mass_init.properties.inertia[1]  = [ 0.0, 0.4, 0.0]
child_vehicle_mass_init.properties.inertia[2]  = [ 0.0, 0.0, 0.4]

dynamics.dyn_manager.add_body_action( child_vehicle_mass_init)

# Set the trans state
child_vehicle_trans_init = trick.castAsjeod__DynBodyInitTransState(trick.TMM_declare_var_s("jeod::DynBodyInitTransState child_vehicle_trans_init"))
child_vehicle_trans_init.set_subject_body( child_vehicle.dyn_body )
child_vehicle_trans_init.reference_ref_frame_name = "Earth.inertial"
child_vehicle_trans_init.body_frame_id            = "composite_body"

#default to "typical" trans-state and lvlh-based orientation
child_vehicle_trans_init.position  = [ -4292653.41, 955168.47, 5139356.57]
child_vehicle_trans_init.velocity  = [ 109.649663, -7527.726490, 1484.521489]

child_vehicle_lvlh_init = trick.castAsjeod__DynBodyInitLvlhRotState(trick.TMM_declare_var_s("jeod::DynBodyInitLvlhRotState child_vehicle_lvlh_init"))
child_vehicle_lvlh_init.set_subject_body( child_vehicle.dyn_body )
child_vehicle_lvlh_init.planet_name                = "Earth"
child_vehicle_lvlh_init.body_frame_id              = "composite_body"
child_vehicle_lvlh_init.orientation.data_source    = trick.Orientation.InputEulerRotation
child_vehicle_lvlh_init.orientation.euler_sequence = trick.Orientation.Yaw_Pitch_Roll
child_vehicle_lvlh_init.orientation.euler_angles   = trick.attach_units( "degree",[ 0.0, -11.6, 0.0])
child_vehicle_lvlh_init.ang_velocity               = [ 0.0, 0.0, 0.0]

dynamics.dyn_manager.add_body_action( child_vehicle_lvlh_init)


dynamics.dyn_manager.add_body_action( child_vehicle_trans_init)

attach_to_parent = trick.castAsjeod__BodyAttachAligned(trick.TMM_declare_var_s("jeod::BodyAttachAligned attach_to_parent"))
attach_to_parent.set_parent_body(vehicle.dyn_body)
attach_to_parent.set_subject_body( child_vehicle.dyn_body )
attach_to_parent.subject_point_name = "child_test_point"
attach_to_parent.parent_point_name = "test_point"

## Attach child_vehicle to vehicle
dynamics.dyn_manager.add_body_action( attach_to_parent )

# Begin maneuver.
trick.add_read(2000.0, """
vehicle.force_extern.force  = [ 0.0, -29000.0, 0.0]
child_vehicle.external_force = [ 0.0, 3210.0, 0.0]
""")
# End maneuver
trick.add_read(2050.0, """
vehicle.force_extern.force  = [ 0.0, 0.0, 0.0]
child_vehicle.external_force = [ 0.0, 0.0, 0.0]
""")

exec(compile(open( "SET_test/chkpt_restart_times.py", "rb").read(), "SET_test/chkpt_restart_times.py", 'exec'))

trick.stop(stop_time)

## Take 2 checkpoints with hexfloat representation
# Drop a checkpoint. NOTE - this line must go after the stop setting
jeod_checkpoint_restart.create_checkpoint (chkpt_restart_time,
                                           stop_time)
trick.checkpoint(second_chkpt_time)

