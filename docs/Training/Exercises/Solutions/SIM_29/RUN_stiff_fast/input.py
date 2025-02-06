exec(compile(open("Common_run/input.py", "rb").read(), "Common_run/input.py", 'exec'))
exec(compile(open("Modified_data/stiff_pair.py", "rb").read(), "Modified_data/stiff_pair.py", 'exec'))

trick.add_read(3.0, """
jeod_time.time_manager.dyn_time.scale_factor = 0.001
""")

trick.add_read(2003.0, """
jeod_time.time_manager.dyn_time.scale_factor = 1.0
""")

trick.stop(2008.0)
