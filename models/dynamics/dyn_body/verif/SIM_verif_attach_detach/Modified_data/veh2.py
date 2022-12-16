veh2.dyn_body.set_name ("veh2")
veh2.dyn_body.integ_frame_name = "central_point.inertial"
veh2.dyn_body.translational_dynamics = True
veh2.dyn_body.rotational_dynamics = True

# Mass-init
veh2.mass_init.action_name = "veh2_mass_init"
veh2.mass_init.subject     = veh2.dyn_body.mass
veh2.mass_init.properties.pt_orientation.data_source = \
                                   trick.Orientation.InputQuaternion
veh2.mass_init.properties.position  = [5.0, 0.0, 0.0]
veh2.mass_init.properties.mass      = 2.0
veh2.mass_init.properties.inertia = [[ 20.0,     0.0,     0.0],
                                     [  0.0,    20.0,     0.0],
                                     [  0.0,     0.0,    20.0]]
veh2.mass_init.num_points = 1
veh2.mass_init.points = trick.sim_services.alloc_type( 1 , "jeod::MassPointInit" )
veh2.mass_init.points[0].set_name ("node21")
veh2.mass_init.points[0].position  = [ 0, 0, 0]
veh2.mass_init.points[0].pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
veh2.mass_init.points[0].pt_orientation.euler_sequence  = \
                                   trick.Orientation.Yaw_Pitch_Roll
veh2.mass_init.points[0].pt_orientation.euler_angles  = \
                                   trick.attach_units( "degree",[ 180.0, 0.0, 0.0])

dynamics.dyn_manager.add_body_action (veh2.mass_init)


# translational state
veh2.trans_init.action_name              = "veh2_trans_init"
veh2.trans_init.dyn_subject                  = veh2.dyn_body
veh2.trans_init.reference_ref_frame_name = "central_point.inertial"
veh2.trans_init.body_frame_id            = "composite_body"
veh2.trans_init.position                 = [  5.0, 10.0, 0.0]
veh2.trans_init.velocity                 = [  0.0,  0.0, 0.0]

dynamics.dyn_manager.add_body_action (veh2.trans_init)


# rotational state
veh2.rot_init.action_name              = "veh2_rot_init"
veh2.rot_init.subject                  = veh2.dyn_body.mass
veh2.rot_init.reference_ref_frame_name = "central_point.inertial"
veh2.rot_init.body_frame_id            = "composite_body"
veh2.rot_init.orientation.data_source  = trick.Orientation.InputEulerRotation
veh2.rot_init.orientation.euler_sequence =trick.Orientation.Yaw_Pitch_Roll
veh2.rot_init.orientation.euler_angles = [ -2.0, 0.0, 0.0 ]
veh2.rot_init.ang_velocity             = [  0.0, 0.0, 0.2]

dynamics.dyn_manager.add_body_action (veh2.rot_init)
