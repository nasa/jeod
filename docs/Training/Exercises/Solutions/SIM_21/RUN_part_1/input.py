# Configure the dynamics manager to operate in empty space mode 
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator

vehicle.dyn_body.set_name("vehA")
vehicle.dyn_body.integ_frame_name = "Space.inertial"
vehicle.dyn_body.translational_dynamics = False
vehicle.dyn_body.rotational_dynamics = False

vehicleB.dyn_body.set_name("vehB")
vehicleB.dyn_body.integ_frame_name = "Space.inertial"
vehicleB.dyn_body.translational_dynamics = False
vehicleB.dyn_body.rotational_dynamics = False

exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
exec(compile(open("Modified_data/vehicleB_mass_props.py", "rb").read(), "Modified_data/vehicleB_mass_props.py", 'exec'))

dynamics.dyn_manager.add_body_action( vehicle.mass_init )
dynamics.dyn_manager.add_body_action( vehicleB.mass_init )

trick.sim_services.exec_set_terminate_time(10.0)
