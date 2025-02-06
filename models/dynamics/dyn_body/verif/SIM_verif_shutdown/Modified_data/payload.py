"""
"  Payload
"""
# Define
payload.primary_body.set_name ("payload_dyn_body")
payload.secondary_mass.set_name ("payload_attached_mass")
payload.primary_body.integ_frame_name = "central_point.inertial"
payload.primary_body.translational_dynamics = True
payload.primary_body.rotational_dynamics = True

# Primary mass init
payload.primary_mass_init.action_name = "payload_primary_mass_init"
payload.primary_mass_init.set_subject_body( payload.primary_body.mass )
payload.primary_mass_init.properties.pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
payload.primary_mass_init.properties.pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
payload.primary_mass_init.properties.position  = [1.0, 2.0, -3.0]
payload.primary_mass_init.properties.mass      = 20.0
payload.primary_mass_init.properties.inertia = [[ 50.0,     0.0,     0.0],
                                                  [  0.0,    50.0,     0.0],
                                                  [  0.0,     0.0,    50.0]]
payload.primary_mass_init.num_points = 2
payload.primary_mass_init.points = \
    trick.sim_services.alloc_type( 2, "jeod::MassPointInit" )
payload.primary_mass_init.points[0].set_name ("vehicle_attach")
payload.primary_mass_init.points[0].position  = [ 0, 0, 0]
payload.primary_mass_init.points[0].pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
payload.primary_mass_init.points[0].pt_orientation.euler_sequence  = \
                                   trick.Orientation.Yaw_Pitch_Roll
payload.primary_mass_init.points[0].pt_orientation.euler_angles  = \
                                   trick.attach_units( "degree",[ 180.0, 0.0, 0.0])
payload.primary_mass_init.points[1].set_name ("other_node")
payload.primary_mass_init.points[1].position  = [ 0, 10, 0]
payload.primary_mass_init.points[1].pt_orientation.data_source = \
                                  trick.Orientation.InputEulerRotation
payload.primary_mass_init.points[1].pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
dynamics.dyn_manager.add_body_action( payload.primary_mass_init )

# Secondary mass init
payload.secondary_mass_init.action_name = "payload_secondary_mass_init"
payload.secondary_mass_init.set_subject_body( payload.secondary_mass )
payload.secondary_mass_init.properties.pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
payload.secondary_mass_init.properties.pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
payload.secondary_mass_init.properties.position  = [-4.0, 0.0, -0.3]
payload.secondary_mass_init.properties.mass      = 2.0
payload.secondary_mass_init.properties.inertia = [[ 20.0,     0.0,     0.0],
                                                  [  0.0,    20.0,     0.0],
                                                  [  0.0,     0.0,    20.0]]
payload.secondary_mass_init.num_points = 4
payload.secondary_mass_init.points = \
        trick.sim_services.alloc_type( 4 , "jeod::MassPointInit" )
payload.secondary_mass_init.points[0].set_name ("attach_point")
payload.secondary_mass_init.points[0].position  = [ 0, 0, 0]
payload.secondary_mass_init.points[0].pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
payload.secondary_mass_init.points[0].pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
payload.secondary_mass_init.points[0].pt_orientation.euler_angles  = \
                                   trick.attach_units( "degree",[ 180.0, 0.0, 0.0])
payload.secondary_mass_init.points[1].set_name ("another_point")
payload.secondary_mass_init.points[1].position  = [ 9, 0.1, -2.1]
payload.secondary_mass_init.points[1].pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
payload.secondary_mass_init.points[1].pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
payload.secondary_mass_init.points[2].set_name ("more_points_that_will_turn_to_body_frame")
payload.secondary_mass_init.points[2].position  = [ 7, -0.5, -6.8]
payload.secondary_mass_init.points[2].pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
payload.secondary_mass_init.points[2].pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
payload.secondary_mass_init.points[3].set_name ("last_one")
payload.secondary_mass_init.points[3].position  = [ 9, 0.1, 2.3]
payload.secondary_mass_init.points[3].pt_orientation.data_source = \
                                   trick.Orientation.InputEulerRotation
payload.secondary_mass_init.points[3].pt_orientation.euler_sequence = \
                                  trick.Orientation.Yaw_Pitch_Roll
dynamics.dyn_manager.add_body_action( payload.secondary_mass_init )

