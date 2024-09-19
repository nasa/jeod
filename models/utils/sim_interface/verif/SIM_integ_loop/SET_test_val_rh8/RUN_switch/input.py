#---------------------------------------------
# Set up Trick executive parameters.
#---------------------------------------------
#trick.sim_services.exec_set_trap_sigfpe(1)

# Set Up Dating Recording
exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(0.1)

# Configure the dynamics manager to operate in empty space mode
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))
set_veh_state(veh_hi_rate, "veh_hi_rate")
set_veh_state(veh_hi_med_rate, "veh_hi_med_rate")
set_veh_state(veh_med_rate, "veh_med_rate")
set_veh_state(veh_med_lo_rate, "veh_med_lo_rate")
set_veh_state(veh_lo_rate, "veh_lo_rate")

trick.add_read(15.0, """
med_rate_loop.integ_loop.add_sim_object(veh_hi_med_rate)
low_rate_loop.integ_loop.add_sim_object(veh_med_lo_rate)
""")
trick.add_read(30.0, """
high_rate_loop.integ_loop.add_sim_object(veh_hi_med_rate)
med_rate_loop.integ_loop.add_sim_object(veh_med_lo_rate)
""")

trick.sim_services.exec_set_terminate_time(33.0)
