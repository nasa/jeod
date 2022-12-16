vehicleB.trans_init.dyn_subject              = vehicleB.dyn_body
vehicleB.trans_init.reference_ref_frame_name = "Space.inertial"
vehicleB.trans_init.body_frame_id            = "composite_body"
vehicleB.trans_init.position                 = [-10.0, -2.0, 0.0]
vehicleB.trans_init.velocity                 = [  2.0,  0.0, 0.0]


vehicleB.rot_init.dyn_subject              =  vehicleB.dyn_body
vehicleB.rot_init.reference_ref_frame_name = "Space.inertial"
vehicleB.rot_init.body_frame_id            = "composite_body"
vehicleB.rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
vehicleB.rot_init.orientation.eigen_angle  = trick.sim_services.attach_units("degree",-90.0)
vehicleB.rot_init.orientation.eigen_axis   = [0, 0, 1] 
vehicleB.rot_init.ang_velocity = trick.sim_services.attach_units("degree/s", [20.0, 0.0, 0.0])
