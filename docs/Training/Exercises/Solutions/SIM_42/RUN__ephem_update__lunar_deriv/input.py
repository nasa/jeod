trick.sim_services.exec_set_trap_sigfpe(1)

exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(30.0*60.0)

#/* Setup general numerical integration parameters. */
exec(compile(open("Modified_data/dynamics.py", "rb").read(), "Modified_data/dynamics.py", 'exec'))

# Specify starting time
exec(compile(open("Modified_data/sim_start.py", "rb").read(), "Modified_data/sim_start.py", 'exec'))

exec(compile(open( "Modified_data/earth_vehicle.py", "rb").read(), "Modified_data/earth_vehicle.py", 'exec'))
exec(compile(open( "Modified_data/moon_vehicle.py", "rb").read(), "Modified_data/moon_vehicle.py", 'exec'))

trick.sim_services.exec_set_terminate_time(86400)
