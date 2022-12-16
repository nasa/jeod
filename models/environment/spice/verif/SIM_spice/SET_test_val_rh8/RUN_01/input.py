trick.sim_services.exec_set_trap_sigfpe(1)

# Setting suppression level to a very large value allows debug
# messages to print. Debug messages are assigned the highest
# number value of the MessageHandler message types, meaning
# they are lowest on the totem pole for being shown.
# jeod_sys.message_handler.set_suppression_level (9999)

# Set logging for the celestial objects in the simulation.
exec(compile(open("../Log_data/earth.dr", "rb").read(), "../Log_data/earth.dr", 'exec'))
exec(compile(open("../Log_data/moon.dr", "rb").read(), "../Log_data/moon.dr", 'exec'))
exec(compile(open("../Log_data/sun.dr", "rb").read(), "../Log_data/sun.dr", 'exec'))
exec(compile(open("../Log_data/mars.dr", "rb").read(), "../Log_data/mars.dr", 'exec'))

# Set the conditions for the simulation.
exec(compile(open("Modified_data/sim_start.py", "rb").read(), "Modified_data/sim_start.py", 'exec'))
exec(compile(open("Modified_data/dynamics.py", "rb").read(), "Modified_data/dynamics.py", 'exec'))
exec(compile(open("Modified_data/spice.py", "rb").read(), "Modified_data/spice.py", 'exec'))
# execfile ("Modified_data/spice_ito_phobos.py")

# Set the simulation stop time.
trick.sim_services.exec_set_terminate_time (1200.0)
