veh3.dyn_body.set_name ("veh3")
veh3.dyn_body.integ_frame_name = "central_point.inertial"
veh3.dyn_body.translational_dynamics = True
veh3.dyn_body.rotational_dynamics = True
veh3.dyn_body.compute_point_derivative = False

# Mass-init
veh3.mass_init.action_name = "veh3_mass_init"
veh3.mass_init.set_subject_body( veh3.dyn_body.mass )
veh3.mass_init.properties.pt_orientation.data_source = \
                                   trick.Orientation.InputQuaternion
veh3.mass_init.properties.position  = [5.0, 0.0, 0.0]
veh3.mass_init.properties.mass      = 3.0
veh3.mass_init.properties.inertia = [[ 30.0,     0.0,     0.0],
                                     [  0.0,    30.0,     0.0],
                                     [  0.0,     0.0,    30.0]]
veh3.mass_init.num_points = 1
veh3.mass_init.points = trick.sim_services.alloc_type( 1 , "jeod::MassPointInit" )
veh3.mass_init.points[0].set_name ("node31")
veh3.mass_init.points[0].position  = [ 0, 0, 5]
veh3.mass_init.points[0].pt_orientation.data_source = \
                                trick.Orientation.InputEulerRotation
veh3.mass_init.points[0].pt_orientation.euler_sequence  = \
                                trick.Orientation.Yaw_Pitch_Roll
veh3.mass_init.points[0].pt_orientation.euler_angles  = \
                                trick.attach_units( "degree",[ 180.0, -90.0, 0.0])

dynamics.dyn_manager.add_body_action (veh3.mass_init)


# translational state
veh3.trans_init.action_name              = "veh3_trans_init"
veh3.trans_init.set_subject_body( veh3.dyn_body.mass )
veh3.trans_init.reference_ref_frame_name = "central_point.inertial"
veh3.trans_init.body_frame_id            = "composite_body"
veh3.trans_init.position                 = [ 0.063, 13.787, -25.0]
veh3.trans_init.velocity                 = [ 0.0,    0.0,     1.0]

dynamics.dyn_manager.add_body_action (veh3.trans_init)


# rotational state
veh3.rot_init.action_name              = "veh3_rot_init"
veh3.rot_init.set_subject_body( veh3.dyn_body )
veh3.rot_init.reference_ref_frame_name = "central_point.inertial"
veh3.rot_init.body_frame_id            = "composite_body"
veh3.rot_init.orientation.data_source  = trick.Orientation.InputEulerRotation
veh3.rot_init.orientation.euler_sequence =trick.Orientation.Yaw_Pitch_Roll
veh3.rot_init.orientation.euler_angles = [ -15.8, 0.0, 0.0 ]
veh3.rot_init.ang_velocity             = [ 0.0, 0.0, 1.0]

dynamics.dyn_manager.add_body_action (veh3.rot_init)
