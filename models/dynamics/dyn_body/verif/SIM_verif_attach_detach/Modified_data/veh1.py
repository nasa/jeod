veh1.dyn_body.set_name ("veh1")
veh1.dyn_body.integ_frame_name = "central_point.inertial"
veh1.dyn_body.translational_dynamics = True
veh1.dyn_body.rotational_dynamics = True

# Mass-init
veh1.mass_init.action_name = "veh1_mass_init"
veh1.mass_init.subject     = veh1.dyn_body.mass
veh1.mass_init.properties.pt_orientation.data_source = \
                                   trick.Orientation.InputQuaternion
veh1.mass_init.properties.position  = [5.0, 0.0, 0.0]
veh1.mass_init.properties.mass      = 1.0
veh1.mass_init.properties.inertia = [[ 10.0,     0.0,     0.0],
                                     [  0.0,    10.0,     0.0],
                                     [  0.0,     0.0,    10.0]]
veh1.mass_init.num_points = 2
veh1.mass_init.points = trick.sim_services.alloc_type( 2 , "jeod::MassPointInit" )
veh1.mass_init.points[0].set_name ("node12")
veh1.mass_init.points[0].position  = [ 10, 0, 0]
veh1.mass_init.points[0].pt_orientation.data_source = \
                                   trick.Orientation.InputQuaternion

veh1.mass_init.points[1].set_name ("node13")
veh1.mass_init.points[1].position  = [ 5, 0, -5]
veh1.mass_init.points[1].pt_orientation.data_source = \
                                trick.Orientation.InputEulerRotation
veh1.mass_init.points[1].pt_orientation.euler_sequence  = \
                                trick.Orientation.Yaw_Pitch_Roll
veh1.mass_init.points[1].pt_orientation.euler_angles  = \
                                trick.attach_units( "degree",[ 0.0, 90.0, 0.0])

dynamics.dyn_manager.add_body_action (veh1.mass_init)


# translational state
veh1.trans_init.action_name              = "veh1_trans_init"
veh1.trans_init.subject                  = veh1.dyn_body.mass
veh1.trans_init.reference_ref_frame_name = "central_point.inertial"
veh1.trans_init.body_frame_id            = "composite_body"
veh1.trans_init.position                 = [ -5.0, 0.0, 0.0]
veh1.trans_init.velocity                 = [  0.0, 1.0, 0.0]

dynamics.dyn_manager.add_body_action (veh1.trans_init)


# rotational state
veh1.rot_init.action_name              = "veh1_rot_init"
veh1.rot_init.dyn_subject                  = veh1.dyn_body
veh1.rot_init.reference_ref_frame_name = "central_point.inertial"
veh1.rot_init.body_frame_id            = "composite_body"
veh1.rot_init.orientation.data_source  = trick.Orientation.InputQuaternion
veh1.rot_init.ang_velocity             = [ 0.0, 0.0, 0.0]

dynamics.dyn_manager.add_body_action (veh1.rot_init)
