exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(1000)

dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_Ephemerides

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator

vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = False

state.solar_beta.reference_name = "Earth"

exec(compile(open("Modified_data/time.py", "rb").read(), "Modified_data/time.py", 'exec'))
exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
exec(compile(open("Modified_data/vehicle_grav_controls.py", "rb").read(), "Modified_data/vehicle_grav_controls.py", 'exec'))
exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))

trick.exec_set_terminate_time(31700000)
