def set_veh_state( Vehicle, veh_name):
  Vehicle.dyn_body.set_name(veh_name)
  Vehicle.dyn_body.integ_frame_name = "Earth.inertial"
  Vehicle.dyn_body.translational_dynamics = True
  Vehicle.dyn_body.rotational_dynamics = True

  Vehicle.trans_init.set_subject_body(Vehicle.dyn_body)
  Vehicle.trans_init.reference_ref_frame_name = "Earth.inertial"
  Vehicle.trans_init.body_frame_id            = "composite_body"
  Vehicle.trans_init.position                 = [7500000.0, 0.0, 0.0]
  Vehicle.trans_init.velocity                 = [0.0 , 7000.0 , 0.0]
  
  Vehicle.rot_init.set_subject_body(Vehicle.dyn_body)
  Vehicle.rot_init.reference_ref_frame_name = "Earth.inertial"
  Vehicle.rot_init.body_frame_id            = "composite_body"
  Vehicle.rot_init.orientation.data_source  = trick.Orientation.InputEigenRotation
  Vehicle.rot_init.orientation.eigen_angle  = 0.0
  Vehicle.rot_init.orientation.eigen_axis   = [0.0, 0.0, 1.0]
  Vehicle.rot_init.ang_velocity             = [0.0, 1.0, 0.0]
  
  Vehicle.mass_init.set_subject_body(Vehicle.dyn_body.mass)
  Vehicle.mass_init.properties.mass =  500.0
  Vehicle.mass_init.properties.position =  [0,0,0]
  Vehicle.mass_init.properties.inertia[0] = [1,0,0]
  Vehicle.mass_init.properties.inertia[1] = [0,1,0]
  Vehicle.mass_init.properties.inertia[2] = [0,0,1]
  Vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
  Vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
  Vehicle.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]

  Vehicle.grav_control.source_name = "Earth"
  Vehicle.grav_control.active = True
  Vehicle.grav_control.spherical = True

  dynamics.dyn_manager.add_body_action( Vehicle.mass_init )
  dynamics.dyn_manager.add_body_action( Vehicle.trans_init )
  dynamics.dyn_manager.add_body_action( Vehicle.rot_init )
  Vehicle.dyn_body.grav_interaction.add_control(Vehicle.grav_control)
  return
