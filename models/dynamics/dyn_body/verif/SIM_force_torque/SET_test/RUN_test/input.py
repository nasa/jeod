#/*******************************************************************************
#Purpose:
#  (Test whether
#    - Forces and torques are properly accumulated
#    - Non-transmitted forces and torques are treated as specified
#    - Equations of motion are properly constructed and integrated.)
#
#
#*******************************************************************************/

# Configure the simulation control parameters.
trick.sim_services.exec_set_trap_sigbus(1)
trick.sim_services.exec_set_trap_sigfpe(1)
trick.sim_services.exec_set_trap_sigsegv(1)

# Turn off unneeded Trick objects
trick_sys.sched.set_sim_object_onoff("trick_vs", 0)
trick_sys.sched.set_sim_object_onoff("instruments", 0)
trick_sys.sched.set_sim_object_onoff("utest", 0)
trick.set_var_server_enabled(0)
# trick.message_device_set_enabled(0)


# Specify logging.
exec(compile(open( "Log_data/log_log_data.py", "rb").read(), "Log_data/log_log_data.py", 'exec'))


# Configure the dynamics manager to operate in empty space mode
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "central_point"
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4


# Define parameters needed to specify the two bodies.
# Body1 is a solid oblate spheroid with
#  - Density = 36.8 kg/m^3,
#  - Polar radius = 5 meters, and
#  - Equatorial radius = 5*sqrt(41/23) meters.
# Body2 is a solid spherical body
#  - Density = 1000 kg/m^3 and
#  - Radius = 1 meter.
# The composite body formed by attaching body2 at a pole of body1 has
# a spherical mass distribution.

pi     = 3.14159265358979323846264338327950288
pix4d3 = 4.0/3.0 * pi

# Magic numbers.
radius_1 =      5.0
radius_2 =      1.0
rho_1    =     36.8
rho_2    =   1000.0
lambda_1 = (41.0/23.0)

# Derived magic numbers.
rsq_1  = radius_1 * radius_1
mass_1 = pix4d3 * rho_1 * lambda_1 * radius_1 * rsq_1
Ixx_1  = 0.2 * mass_1 * (1.0+lambda_1) * rsq_1
Izz_1  = 0.4 * mass_1 * lambda_1 * rsq_1

rsq_2  = radius_2 * radius_2
mass_2 = pix4d3 * rho_2 * radius_2 * rsq_2
Izz_2  = 0.4 * mass_2 * rsq_2


# When attached as described the composite body should be a body with
#  - Total mass = sum of the individual masses
#  - Composite CoM to body 2 CoM distance = mass_1/mass_c * core CoM seperation
#  - Spherical mass distribution with inertia = sum of z moments of inertia
mass_c = mass_1 + mass_2
zpos_2 = mass_1/mass_c * (radius_1 + radius_2)
Izz_c  = Izz_1 + Izz_2


# Prepare to make the simulated objects act in a predictable manner.
# The goal is to apply forces and torques that result in behaviors that
# make it easy to come up with comparison data and yet test functionality.

# First define forces that yield 1 m/s^2 accelerations.
# force_combined yields a 1 m/s^2 accel when bodies 1&2 are combined.
# force_body2    yields a 1 m/s^2 accel on body2 when separated from body1.
force_combined = mass_c
force_body2    = mass_2

# A force along body2's structural x-y plane and through its center of mass
# will induce a torque on the composite body when body2 is attached to body1.
# Calculate the compensating torque needed to null out this cross-coupling
# assuming a force of magnitude force_combined.
torque_compensating = zpos_2 * force_combined

# During the rotation phases of the test the composite body is to
# accelerate rotationally for 10 seconds, then counter-accelerate for
# another 10 seconds. The goal is a 90 degree change in orientation.
ang_accel = 0.5 * pi / 100
torque_rot_90 = Izz_c * ang_accel


# Initialize body1.
body1.dyn_body.integ_frame_name = "central_point.inertial"
body1.dyn_body.translational_dynamics = True
body1.dyn_body.rotational_dynamics = True
body1.dyn_body.set_name("body1")


# Define body1 mass properties, including the attach point.
# The structural origin is at the intersection of the polar (z) axis of the
# ellipsoid and the surface and is opposite the attach point.
# The positive z structural axis points toward the CoM.
body1.mass_init.set_subject_body( body1.dyn_body.mass )
body1.mass_init.action_name = "Body1_mass_init"
body1.mass_init.properties.mass  = mass_1
body1.mass_init.properties.position  = [ 0.0, 0.0, radius_1]

body1.mass_init.properties.inertia_spec = trick.MassPropertiesInit.Body
body1.mass_init.properties.inertia[0]  = [ Ixx_1,   0.0,   0.0]
body1.mass_init.properties.inertia[1]  = [ 0.0,   Ixx_1,   0.0]
body1.mass_init.properties.inertia[2]  = [ 0.0,     0.0, Izz_1]

# Body1's body axes are a 180 yaw from the structural axes.
body1.mass_init.properties.pt_orientation.data_source = \
                                    trick.Orientation.InputEulerRotation
body1.mass_init.properties.pt_orientation.euler_sequence  = \
                                    trick.Orientation.Yaw_Pitch_Roll
body1.mass_init.properties.pt_orientation.euler_angles  = \
                                    trick.attach_units( "degree",[ 180.0, 0.0, 0.0])

# Define the point on body1 at which body2 will attach to body1.
# The body1 attach point is diagonally opposed to the structural origin,
# with the docking x-axis along the outward normal.
body1.mass_init.allocate_points(1)

body1.mass_init.get_mass_point(0).set_name("port")
body1.mass_init.get_mass_point(0).position  = [ 0.0, 0.0, 2*radius_1]
body1.mass_init.get_mass_point(0).pt_orientation.data_source =  \
                                                trick.Orientation.InputMatrix
body1.mass_init.get_mass_point(0).pt_orientation.trans[0]  = [ 0.0, 0.0, 1.0]
body1.mass_init.get_mass_point(0).pt_orientation.trans[1]  = [ 1.0, 0.0, 0.0]
body1.mass_init.get_mass_point(0).pt_orientation.trans[2]  = [ 0.0, 1.0, 0.0]

dynamics.dyn_manager.add_body_action (body1.mass_init)



# Initialize body1's translational state:
# Put the composite CoM at rest at the origin.
body1.trans_init.set_subject_body( body1.dyn_body )
body1.trans_init.reference_ref_frame_name = "central_point.inertial"
body1.trans_init.body_frame_id = "composite_body"
body1.trans_init.position  = [ 0.0, 0.0, 0.0]
body1.trans_init.velocity  = [ 0.0, 0.0, 0.0]

dynamics.dyn_manager.add_body_action (body1.trans_init)



# Initialize body1's rotational state:
# Zero rate, structure is a 180 degree pitch from the inertial frame.
# The inertial to body transformation matrix should be a 180 degree roll.
body1.rot_init.reference_ref_frame_name = "central_point.inertial"
body1.rot_init.body_frame_id = "structure"
body1.rot_init.set_subject_body( body1.dyn_body )
body1.rot_init.ang_velocity  = trick.attach_units( "rad/s",[ 0.0, 0.0, 0.0])
body1.rot_init.orientation.data_source = trick.Orientation.InputEulerRotation
body1.rot_init.orientation.euler_sequence  = trick.Orientation.Pitch_Yaw_Roll
body1.rot_init.orientation.euler_angles  = \
                                  trick.attach_units( "degree",[  180.0, 0.0, 0.0])

dynamics.dyn_manager.add_body_action (body1.rot_init)



# Initialize body2.
body2.dyn_body.integ_frame_name = "central_point.inertial"
body2.dyn_body.translational_dynamics = True
body2.dyn_body.rotational_dynamics = True
body2.dyn_body.set_name("body2")


# Define body2 mass properties, including the attach point.
# The structural origin is on the surface of the sphere.
# The positive z structural axis points toward the CoM.
body2.mass_init.set_subject_body( body2.dyn_body.mass )
body2.mass_init.action_name = "Body2_mass_init"
body2.mass_init.properties.mass  = mass_2
body2.mass_init.properties.position  = [ 0.0, 0.0, radius_2]
body2.mass_init.properties.inertia_spec = trick.MassPropertiesInit.Body
body2.mass_init.properties.inertia[0]  = [ Izz_2,   0.0,   0.0]
body2.mass_init.properties.inertia[1]  = [ 0.0,   Izz_2,   0.0]
body2.mass_init.properties.inertia[2]  = [ 0.0,     0.0, Izz_2]

# Body2's body axes are a 180 yaw from the structural axes.
body2.mass_init.properties.pt_orientation.data_source = \
                                    trick.Orientation.InputEulerRotation
body2.mass_init.properties.pt_orientation.euler_sequence  = \
                                    trick.Orientation.Yaw_Pitch_Roll
body2.mass_init.properties.pt_orientation.euler_angles  = \
                                    trick.attach_units( "degree",[ 180.0, 0.0, 0.0])

# Define the point on body2 at which body2 will attach to body1.
# The body2 attach point is diagonally opposed to the structural origin,
# with the docking x-axis along the outward normal.
body2.mass_init.allocate_points(1)

body2.mass_init.get_mass_point(0).set_name("port")
body2.mass_init.get_mass_point(0).position  = [ 0.0, 0.0, 2*radius_2]
body2.mass_init.get_mass_point(0).pt_orientation.data_source = \
                                               trick.Orientation.InputMatrix
body2.mass_init.get_mass_point(0).pt_orientation.trans[0]  = [ 0.0, 0.0, 1.0]
body2.mass_init.get_mass_point(0).pt_orientation.trans[1]  = [ 1.0, 0.0, 0.0]
body2.mass_init.get_mass_point(0).pt_orientation.trans[2]  = [ 0.0, 1.0, 0.0]

dynamics.dyn_manager.add_body_action (body2.mass_init)



# Attach body2 to body1 at the predefined attach points.
# This attachment is to take place before initializing state.
# The composite body should have a spherical mass distribution.
body2.attach.action_name = "attach"
body2.attach.set_subject_body( body2.dyn_body )
body2.attach.set_parent_body( body1.dyn_body )
body2.attach.subject_point_name = "port"
body2.attach.parent_point_name =  "port"
body2.attach.active = True

dynamics.dyn_manager.add_body_action (body2.attach)



# Queue the detachment of body2 from body1.
body2.detach.set_subject_body( body2.dyn_body )
body2.detach.active = False

dynamics.dyn_manager.add_body_action (body2.detach)



# From t=0 to t=3 build up the z-axis acceleration to 3 m/s^2 in steps of 1.
trick.add_read( 1, """
body2.effector_forc1.force  = [ 0.0, 0.0, force_combined]
""")
trick.add_read( 2, """
body2.environ_forc1.force  = [ 0.0, 0.0, force_combined]
""")
trick.add_read( 3, """
body2.environ_forc2.force  = [ 0.0, 0.0, force_combined]
""")

# Now start playing games with the non-transmitted forces.
# The composite body acceleration should remain constant at 3 m/s^2.
trick.add_read( 4, """
body2.no_xmit_forc1.force  = [ 0.0, 0.0, -force_combined]
""")
trick.add_read( 5, """
body2.no_xmit_forc2.force  = [ 0.0, 0.0, -force_combined]
""")
trick.add_read( 6, """
body2.no_xmit_forc3            = [ 0.0, 0.0, -force_combined]
""")

# Detach body2 at t=7. There are no forces on body1 it just drifts.
# Make body2 zoom back to its original location in 2 seconds, but with
# no net change in velocity.
trick.add_read( 7, """
body2.detach.active = True
body2.effector_forc1.force  = [ 0.0, 0.0, 0.0]
body2.no_xmit_forc1.force  = [ 0.0, 0.0, -68.5*force_body2]

# Queue a reattach, but do not active.
body2.attach.active = False
dynamics.dyn_manager.add_body_action (body2.attach)
""")


# This just reverses the acceleration from t=7 to t=8 so that at t=9
# body2 has reached its origin with no net change in velocity.
trick.add_read( 8, """
body2.environ_forc1.force  = [ 0.0, 0.0, 0.0]
body2.environ_forc2.force  = [ 0.0, 0.0, 0.0]
body2.no_xmit_forc1.force  = [ 0.0, 0.0, 68.5*force_body2]
body2.no_xmit_forc2.force  = [ 0.0, 0.0, 0.0]
body2.no_xmit_forc3        = [ 0.0, 0.0, 0.0]
""")


# At t=9 enable the re-attachment. Body2 should teleport back to body1.
# This tests JEOD's magical teleport docking capability.
# At the same time, apply non-transmitted forces to body1 to eventually
# make the body start moving back toward the origin.
trick.add_read( 9, """
body2.attach.active = True
body2.no_xmit_forc1.force  = [ 0.0, 0.0, 0.0]
body1.no_xmit_forc1.force  = [ 0.0, 0.0, 3*force_combined]
""")

# The object is now moving back toward the origin.
# start cancelling the velocity.
trick.add_read( 19, """
body1.no_xmit_forc1.force  = [ 0.0, 0.0, 0.0]
""")

trick.add_read( 21, """
body1.no_xmit_forc1.force  = [ 0.0, 0.0, -3*force_combined]
""")

trick.add_read( 25, """
body1.no_xmit_forc1.force  = [ 0.0, 0.0, -2*force_combined]
""")

trick.add_read( 26, """
body1.no_xmit_forc1.force  = [ 0.0, 0.0, -force_combined]
""")

# At t=27 we should be right back to the origin. Take a break for a few secs.
trick.add_read( 27, """
body1.no_xmit_forc1.force  = [ 0.0, 0.0, 0.0]
""")


# Now take a little jaunt along the x-axis.
# Start with a 1 m/s^2 equiv. force along body2's structural x-axis.
# Body 2's axes are currently aligned with inertial.
# An x-axis force through body2's CoM will produce a torque on the
# composite body. To maintain orientation, a compensating torque is needed.
# For funsies, add a non-transmitted torque from body2.
# Maintain this for two seconds.
trick.add_read( 30, """
body2.environ_forc1.force   = [ force_combined, 0.0, 0.0]
body1.no_xmit_torq1.torque  = [ 0.0, torque_compensating, 0.0]
body2.no_xmit_torq1.torque  = [ 0.0, torque_compensating, 0.0]
""")

# At t=32 turn the x-axis force and compensating torques off and
# start rotating about about body2's structural z-axis.
# The goal is to accomplish a 90 degree yaw.
trick.add_read( 32, """
body2.environ_forc1.force   = [ 0.0, 0.0, 0.0]
body1.no_xmit_torq1.torque  = [ 0.0, 0.0, 0.0]
body2.no_xmit_torq1.torque  = [ 0.0, 0.0, 0.0]
body2.environ_torq1.torque  = [ 0.0, 0.0, torque_rot_90]
""")

trick.add_read( 42, """
body2.environ_torq1.torque   = [ 0.0, 0.0, 0.0]
body2.effector_torq1.torque  = [ 0.0, 0.0, -torque_rot_90]
""")

# At t=52 the 90 degree yaw is complete. Cancel the yaw torque.
# At the same time, start returning to the origin, once again using a
# force on body2 plus a compensating torque. Due to the 90 degree yaw,
# body2's structural y-axis points along -x inertial.
trick.add_read( 52, """
body2.effector_torq1.torque  = [ 0.0, 0.0, 0.0]
body2.environ_forc1.force    = [ 0.0, force_combined, 0.0]
body1.no_xmit_torq1.torque   = [ torque_compensating, 0.0, 0.0]
""")

# At t=56 we have reversed the velocity vector. Cancel the y burn.
# At the same time, start a 90 roll about body2 structural x.
trick.add_read( 56, """
body2.environ_forc1.force   = [ 0.0, 0.0, 0.0]
body1.no_xmit_torq1.torque  = [ 0.0, 0.0, 0.0]
body2.environ_torq1.torque  = [ torque_rot_90, 0.0, 0.0]
""")

trick.add_read( 66, """
body2.environ_torq1.torque  = [ -torque_rot_90, 0.0, 0.0]
""")

# At t=76 the 90 degree roll is complete. Stop the roll.
# At the same time, work toward bring the body to rest at the origin.
# The composite CoM should be 2 meters from the origin and moving toward
# the origin at 2 m/s. Applying a -1 m/s^2 acceleration along the inertial
# x-axis will do the trick.
trick.add_read( 76, """
body2.environ_torq1.torque = [ 0.0, 0.0, 0.0]
body1.no_xmit_forc1.force  = [ 0.0, 0.0, -force_combined]
""")

trick.add_read( 78, """
body1.no_xmit_forc1.force  = [ 0.0, 0.0, 0.0]
""")


trick.stop(80)
