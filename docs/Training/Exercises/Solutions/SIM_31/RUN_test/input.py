exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(1)

dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "Sun"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator

vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Sun.inertial"
vehicle.dyn_body.translational_dynamics = False
vehicle.dyn_body.rotational_dynamics = False

vehicle.array.set_name("array")

exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))
exec(compile(open("Modified_data/surface.py", "rb").read(), "Modified_data/surface.py", 'exec'))
exec(compile(open("Modified_data/attach.py", "rb").read(), "Modified_data/attach.py", 'exec'))

vehicle.facet_pos = [0,0,0]
vehicle.T_for_rotation = [[1,0,0],[0,1,0],[0,0,1]]

trick.exec_set_terminate_time(1000)
