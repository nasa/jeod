exec(compile(open("Common_run/input.py", "rb").read(), "Common_run/input.py", 'exec'))
exec(compile(open("Modified_data/basic_pair.py", "rb").read(), "Modified_data/basic_pair.py", 'exec'))

trick.sim_services.exec_set_terminate_time(10.0)
