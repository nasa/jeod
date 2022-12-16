vehicleB.trans_init.dyn_subject              = vehicleB.dyn_body
vehicleB.trans_init.reference_ref_frame_name = "Space.inertial"
vehicleB.trans_init.body_frame_id            = "composite_body"
vehicleB.trans_init.position                 = [ 0.0 , -5.0 , 0]
vehicleB.trans_init.velocity                 = [ 0.0 , 1.0 , 0]


vehicleB.rot_init.dyn_subject              = vehicleB.dyn_body
vehicleB.rot_init.reference_ref_frame_name = "Space.inertial"
vehicleB.rot_init.body_frame_id            = "composite_body"
vehicleB.rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
vehicleB.rot_init.orientation.eigen_angle  = 0.0
vehicleB.rot_init.orientation.eigen_axis   = [0.0,0.0,1.0] 
vehicleB.rot_init.ang_velocity             = [0.0, 0.0, 0.0]


dynamics.dyn_manager.add_body_action( vehicleB.trans_init )
dynamics.dyn_manager.add_body_action( vehicleB.rot_init )
