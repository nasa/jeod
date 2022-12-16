# Set Up Dating Recording
exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(1.0)

# Configure the dynamics manager to operate in empty space mode 
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator

vehicle.dyn_body.set_name("vehA")
vehicle.dyn_body.integ_frame_name = "Space.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = True

vehicleB.dyn_body.set_name("vehB")
vehicleB.dyn_body.integ_frame_name = "Space.inertial"
vehicleB.dyn_body.translational_dynamics = True
vehicleB.dyn_body.rotational_dynamics = True

exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))
exec(compile(open("Modified_data/vehicleB_mass_props.py", "rb").read(), "Modified_data/vehicleB_mass_props.py", 'exec'))
exec(compile(open("Modified_data/vehicleB_state.py", "rb").read(), "Modified_data/vehicleB_state.py", 'exec'))

dynamics.dyn_manager.add_body_action( vehicle.mass_init )
dynamics.dyn_manager.add_body_action( vehicle.trans_init )
dynamics.dyn_manager.add_body_action( vehicle.rot_init )


dynamics.dyn_manager.add_body_action( vehicleB.mass_init )
dynamics.dyn_manager.add_body_action( vehicleB.trans_init )
dynamics.dyn_manager.add_body_action( vehicleB.rot_init )

trick.add_read(5.0 , """
mass_attach = trick.BodyAttachAligned()
mass_attach.thisown = 0
mass_attach.dyn_subject = vehicle.dyn_body
mass_attach.dyn_parent  = vehicleB.dyn_body
mass_attach.subject_point_name = "Vehicle A Mass Point 1"
mass_attach.parent_point_name  = "Vehicle B Mass Point 1"
dynamics.dyn_manager.add_body_action( mass_attach)
mass_attach.active = True
dynamics.dyn_manager.perform_actions()
""")

trick.add_read(15.0 , """
vehicle.dyn_body.detach()
""")

trick.sim_services.exec_set_terminate_time(20.0)
