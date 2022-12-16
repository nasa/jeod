exec(compile(open( "Log_data/log_veh_state_rec.py", "rb").read(), "Log_data/log_veh_state_rec.py", 'exec'))

exec(compile(open("Modified_data/integration.py", "rb").read(), "Modified_data/integration.py", 'exec'))
exec(compile(open("Modified_data/time.py", "rb").read(), "Modified_data/time.py", 'exec'))
exec(compile(open("Modified_data/vehicle.py", "rb").read(), "Modified_data/vehicle.py", 'exec'))
trick.stop(100)
