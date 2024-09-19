trick.sim_services.exec_set_trap_sigfpe(1)

jeod_sys.interface_init.message_suppression_level = trick.MessageHandler.Debug

# Set logging for the celestial objects in the simulation.
exec(compile(open("../Log_data/earth.py", "rb").read(), "../Log_data/earth.py", 'exec'))
exec(compile(open("../Log_data/moon.py", "rb").read(), "../Log_data/moon.py", 'exec'))
exec(compile(open("../Log_data/sun.py", "rb").read(), "../Log_data/sun.py", 'exec'))
exec(compile(open("../Log_data/mars.py", "rb").read(), "../Log_data/mars.py", 'exec'))

# Set the conditions for the simulation.
exec(compile(open("Modified_data/sim_start.py", "rb").read(), "Modified_data/sim_start.py", 'exec'))
exec(compile(open("Modified_data/dynamics.py", "rb").read(), "Modified_data/dynamics.py", 'exec'))
exec(compile(open("Modified_data/spice.py", "rb").read(), "Modified_data/spice.py", 'exec'))
# exec(open("Modified_data/spice_ito_phobos.py").read())

# Set the simulation stop time.
trick.sim_services.exec_set_terminate_time (1200.0)
