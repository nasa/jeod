trick.sim_services.exec_set_trap_sigfpe(1)

# Set logging for the celestial objects in the simulation.
exec(compile(open("../Log_data/earth.dr", "rb").read(), "../Log_data/earth.dr", 'exec'))
exec(compile(open("../Log_data/moon.dr", "rb").read(), "../Log_data/moon.dr", 'exec'))
exec(compile(open("../Log_data/mars.dr", "rb").read(), "../Log_data/mars.dr", 'exec'))
exec(compile(open("../Log_data/sun.dr", "rb").read(), "../Log_data/sun.dr", 'exec'))

# Set the conditions for the simulation.
exec(compile(open("Modified_data/sim_start.py", "rb").read(), "Modified_data/sim_start.py", 'exec'))
exec(compile(open("Modified_data/dynamics.py", "rb").read(), "Modified_data/dynamics.py", 'exec'))

# Set the simulation stop time.
trick.sim_services.exec_set_terminate_time (1200.0)
