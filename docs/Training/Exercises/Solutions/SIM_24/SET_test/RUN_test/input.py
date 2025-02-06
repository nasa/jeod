#---------------------------------------------
# Set up Trick executive parameters.
#---------------------------------------------
#trick.sim_services.exec_set_trap_sigfpe(1)

# Set Up Dating Recording
exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(1.0)

# Configure the dynamics manager to operate in single planet mode 
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "Earth"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator


vehicle.dyn_body.set_name("veh")
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = True

vehicle2.dyn_body.set_name("veh2")
vehicle2.dyn_body.integ_frame_name = "Earth.inertial"
vehicle2.dyn_body.translational_dynamics = True
vehicle2.dyn_body.rotational_dynamics = True

exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
exec(compile(open("Modified_data/vehicle_grav_controls.py", "rb").read(), "Modified_data/vehicle_grav_controls.py", 'exec'))
exec(compile(open("Modified_data/vehicle_state_orbelm.py", "rb").read(), "Modified_data/vehicle_state_orbelm.py", 'exec'))
exec(compile(open("Modified_data/vehicle2_state_lvlh.py", "rb").read(), "Modified_data/vehicle2_state_lvlh.py", 'exec'))
grav_ctrl(vehicle)
grav_ctrl(vehicle2)
mass_props(vehicle)
mass_props(vehicle2)


trick.sim_services.exec_set_terminate_time(18000.0);
