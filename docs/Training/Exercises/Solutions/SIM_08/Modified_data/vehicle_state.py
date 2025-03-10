vehicle.trans_init.set_subject_body(vehicle.dyn_body)
vehicle.trans_init.reference_ref_frame_name = "Space.inertial"
vehicle.trans_init.body_frame_id            = "composite_body"
vehicle.trans_init.position                 = [10 , 0 , 0]
vehicle.trans_init.velocity                 = [-2 , 0 , 0]


vehicle.rot_init.set_subject_body(vehicle.dyn_body)
vehicle.rot_init.reference_ref_frame_name = "Space.inertial"
vehicle.rot_init.body_frame_id            = "composite_body"
vehicle.rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
vehicle.rot_init.orientation.eigen_angle  = trick.sim_services.attach_units("degree",180.0)
vehicle.rot_init.orientation.eigen_axis   = [0.0, 0.0, 1.0] 
vehicle.rot_init.ang_velocity             = trick.sim_services.attach_units("degree/s" , [0.0, 0.0, -36.0])
