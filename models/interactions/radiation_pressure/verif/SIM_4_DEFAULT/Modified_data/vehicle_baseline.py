################TRICK HEADER#######################################
#PURPOSE:
#  (To define the method set_vehicle_baseline
####################################################################################

vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Sun.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = True


vehicle.mass_init.set_subject_body( vehicle.dyn_body.mass )
vehicle.mass_init.properties.pt_orientation.data_source =    trick.Orientation.InputEigenRotation


vehicle.mass_init.properties.pt_orientation.eigen_angle  = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [ 0, 1, 0]
vehicle.mass_init.properties.mass  = 300
vehicle.mass_init.properties.position  = [ 0.0, 0.0, 0.0]
vehicle.mass_init.properties.inertia[0]  = [ 100.0,  0.0,   0.0]
vehicle.mass_init.properties.inertia[1]  = [ 0.0,    200.0, 0.0]
vehicle.mass_init.properties.inertia[2]  = [ 0.0,    0.0,   400.0]


dynamics.dyn_manager.add_body_action (vehicle.mass_init)


vehicle.trans_init.set_subject_body( vehicle.dyn_body )
vehicle.trans_init.reference_ref_frame_name = "Sun.inertial"
vehicle.trans_init.body_frame_id = "composite_body"
vehicle.trans_init.position  = [ 1.5E11, 0, 0]
vehicle.trans_init.velocity  = [ 0,0,0]

dynamics.dyn_manager.add_body_action (vehicle.trans_init)


vehicle.rot_init.set_subject_body( vehicle.dyn_body )
vehicle.rot_init.reference_ref_frame_name = "Sun.inertial"
vehicle.rot_init.body_frame_id = "composite_body"
vehicle.rot_init.ang_velocity  = [ 0,0, 0.07]
vehicle.rot_init.orientation.data_source = trick.Orientation.InputMatrix

vehicle.rot_init.orientation.trans[0]  = [ 1.0, 0.0, 0.0]
vehicle.rot_init.orientation.trans[1]  = [ 0.0, 1.0, 0.0]
vehicle.rot_init.orientation.trans[2]  = [ 0.0, 0.0, 1.0]


dynamics.dyn_manager.add_body_action (vehicle.rot_init)

vehicle.grav_control.source_name   = "Sun"
vehicle.grav_control.active        = 0
vehicle.grav_control.spherical     = 1

vehicle.dyn_body.grav_interaction.add_control(vehicle.grav_control)
