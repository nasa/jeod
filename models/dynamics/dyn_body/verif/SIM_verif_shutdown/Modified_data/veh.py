"""
"  Vehicle
"""
veh.dyn_body.set_name ("vehicle")
veh.dyn_body.integ_frame_name = "central_point.inertial"
veh.dyn_body.translational_dynamics = True
veh.dyn_body.rotational_dynamics = True

# Mass-init
veh.mass_init.action_name = "veh_mass_init"
veh.mass_init.set_subject_body( veh.dyn_body.mass )
veh.mass_init.properties.pt_orientation.data_source = \
                                  trick.Orientation.InputEulerRotation
veh.mass_init.properties.pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
veh.mass_init.properties.position  = [5.0, 0.0, 0.0]
veh.mass_init.properties.mass      = 1.0
veh.mass_init.properties.inertia = [[ 10.0,     0.0,     0.0],
                                    [  0.0,    10.0,     0.0],
                                    [  0.0,     0.0,    10.0]]
veh.mass_init.allocate_points(3)

veh.mass_init.get_mass_point(0).set_name ("payload_attach")
veh.mass_init.get_mass_point(0).position  = [ 10, 0, 0]
veh.mass_init.get_mass_point(0).pt_orientation.data_source = \
                                  trick.Orientation.InputEulerRotation
veh.mass_init.get_mass_point(0).pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
veh.mass_init.get_mass_point(1).set_name ("other_point")
veh.mass_init.get_mass_point(1).position  = [ 5, 0, -5]
veh.mass_init.get_mass_point(1).pt_orientation.data_source = \
                               trick.Orientation.InputEulerRotation
veh.mass_init.get_mass_point(1).pt_orientation.euler_sequence  = \
                               trick.Orientation.Yaw_Pitch_Roll
veh.mass_init.get_mass_point(1).pt_orientation.euler_angles  = \
                                trick.attach_units( "degree",[ 0.0, 90.0, 0.0])
veh.mass_init.get_mass_point(2).set_name ("third_point")
veh.mass_init.get_mass_point(2).position  = [ 5, 0, -5]
veh.mass_init.get_mass_point(2).pt_orientation.data_source = \
                                  trick.Orientation.InputEulerRotation
veh.mass_init.get_mass_point(2).pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
dynamics.dyn_manager.add_body_action( veh.mass_init )

# translational state
veh.trans_init.action_name              = "veh_trans_init"
veh.trans_init.set_subject_body( veh.dyn_body.mass )
veh.trans_init.reference_ref_frame_name = "central_point.inertial"
veh.trans_init.body_frame_id            = "composite_body"
veh.trans_init.position                 = [ -5.0, 0.0, 0.0]
veh.trans_init.velocity                 = [  0.0, 1.0, 0.0]
dynamics.dyn_manager.add_body_action (veh.trans_init)

# rotational state
veh.rot_init.action_name              = "veh_rot_init"
veh.rot_init.set_subject_body( veh.dyn_body )
veh.rot_init.reference_ref_frame_name = "central_point.inertial"
veh.rot_init.body_frame_id            = "composite_body"
veh.rot_init.orientation.data_source  = trick.Orientation.InputEulerRotation
veh.rot_init.orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
veh.rot_init.ang_velocity             = [ 0.0, 0.0, 0.0]
dynamics.dyn_manager.add_body_action (veh.rot_init)


