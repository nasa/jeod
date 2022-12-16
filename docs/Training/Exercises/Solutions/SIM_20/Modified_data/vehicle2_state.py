vehicle2.trans_init.dyn_subject              = vehicle2.dyn_body
vehicle2.trans_init.reference_ref_frame_name = "Space.inertial"
vehicle2.trans_init.body_frame_id            = "composite_body"
vehicle2.trans_init.position                 = [-10 , 0 , 0]
vehicle2.trans_init.velocity                 = [  2 , 1 , 0]


vehicle2.rot_init.dyn_subject              = vehicle2.dyn_body
vehicle2.rot_init.reference_ref_frame_name = "Space.inertial"
vehicle2.rot_init.body_frame_id            = "composite_body"
vehicle2.rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
vehicle2.rot_init.orientation.eigen_angle  = trick.sim_services.attach_units("degree",180.0)
vehicle2.rot_init.orientation.eigen_axis   = [0.0, 0.0, 1.0] 
vehicle2.rot_init.ang_velocity             = [0.0, 0.0, 0.0]
