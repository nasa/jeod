exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(1)

dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "Earth"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator

vehicleA.dyn_body.set_name("vehicleA")
vehicleA.dyn_body.integ_frame_name = "Earth.inertial"
vehicleA.dyn_body.translational_dynamics = True
vehicleA.dyn_body.rotational_dynamics = True
vehicleB.dyn_body.set_name("vehicleB")
vehicleB.dyn_body.integ_frame_name = "Earth.inertial"
vehicleB.dyn_body.translational_dynamics = True
vehicleB.dyn_body.rotational_dynamics = True

exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
exec(compile(open("Modified_data/vehicle_grav_controls.py", "rb").read(), "Modified_data/vehicle_grav_controls.py", 'exec'))
exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))
exec(compile(open("Modified_data/derived_state.py", "rb").read(), "Modified_data/derived_state.py", 'exec'))
mass_props(vehicleA)
mass_props(vehicleB)

trick.exec_set_terminate_time(6000)
