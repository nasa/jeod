#---------------------------------------------
# Set up Trick executive parameters.
#---------------------------------------------
#trick.sim_services.exec_set_trap_sigfpe(1)

# Set Up Dating Recording
exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(0.1)

# Configure the dynamics manager to operate in empty space mode
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_SinglePlanet
dynamics.dyn_manager_init.central_point_name = "Earth"

exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))
set_veh_state(vehicle,  "veh1")
set_veh_state(vehicle2, "veh2")
set_veh_state(vehicle3, "veh3")

trick.add_read(400.0, "med.integ_loop.add_sim_object(vehicle)")

trick.sim_services.exec_set_terminate_time(500.0)
