#---------------------------------------------
# Set up Trick executive parameters.
#---------------------------------------------
#trick.sim_services.exec_set_trap_sigfpe(1)

# Configure the dynamics manager to operate in empty space mode
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator


vehicle.dyn_body.set_name("veh")
vehicle.dyn_body.integ_frame_name = "Space.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = True


vehicle.mass_init.set_subject_body( vehicle.dyn_body.mass )
# note all units default to SI (mks) units
vehicle.mass_init.properties.mass = 10.0
vehicle.mass_init.properties.position = [0,0,0]
vehicle.mass_init.properties.inertia[0] = [1,0,0]
vehicle.mass_init.properties.inertia[1] = [0,1,0]
vehicle.mass_init.properties.inertia[2] = [0,0,1]
vehicle.mass_init.properties.pt_orientation.data_source = trick.Orientation.InputEigenRotation
vehicle.mass_init.properties.pt_orientation.eigen_angle = 0.0
vehicle.mass_init.properties.pt_orientation.eigen_axis  = [0.0, 1.0, 0.0]



vehicle.trans_init.set_subject_body( vehicle.dyn_body )
vehicle.trans_init.reference_ref_frame_name = "Space.inertial"
vehicle.trans_init.body_frame_id 	    = "composite_body"
vehicle.trans_init.position  = [0.0 , 0.0 , 0.0]
vehicle.trans_init.velocity  = [0.0 , 1.0 , 0.0]



vehicle.rot_init.set_subject_body( vehicle.dyn_body )
vehicle.rot_init.reference_ref_frame_name    = "Space.inertial"
vehicle.rot_init.body_frame_id		     = "composite_body"
vehicle.rot_init.orientation.data_source     = trick.Orientation.InputEigenRotation
vehicle.rot_init.orientation.eigen_angle     = 0
vehicle.rot_init.orientation.eigen_axis      = [0,0,1]
vehicle.rot_init.ang_velocity		     = [0.0, 0.0, 0.0]


exec(compile(open("Modified_data/data.py", "rb").read(), "Modified_data/data.py", 'exec'))

dynamics.dyn_manager.add_body_action( vehicle.mass_init )
dynamics.dyn_manager.add_body_action( vehicle.trans_init )
dynamics.dyn_manager.add_body_action( vehicle.rot_init )

#forces_object.extern_forc = [5.0, 0, 0]

trick.stop(1000.0);
