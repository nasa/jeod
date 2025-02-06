vehicle.trans_init.set_subject_body(vehicle.dyn_body)
vehicle.trans_init.reference_ref_frame_name = "Earth.inertial"
vehicle.trans_init.body_frame_id 	    = "composite_body"
vehicle.trans_init.position  = trick.sim_services.attach_units("M" , [7400000.0, 0.0, 0.0])
vehicle.trans_init.velocity  = trick.sim_services.attach_units("M/s" , [0.0 , 7300.0 , 758.0])

vehicle.rot_init.set_subject_body(vehicle.dyn_body)
vehicle.rot_init.reference_ref_frame_name = "Earth.inertial"
vehicle.rot_init.body_frame_id            = "composite_body"
vehicle.rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
vehicle.rot_init.orientation.eigen_angle  = trick.sim_services.attach_units("degree",0.0)
vehicle.rot_init.orientation.eigen_axis   = [0.0,0.0,1.0]
vehicle.rot_init.ang_velocity	     = trick.sim_services.attach_units("degree/s" , [0.0, 0.0, 0.0])
