exec(compile(open("RUN_gravonly/input.py", "rb").read(), "RUN_gravonly/input.py", 'exec'))
interactions.aero_drag.active = True
interactions.aero_drag.use_default_behavior = False
interactions.aero_drag.set_aero_surface(interactions.aero_surface)

interactions.aero_drag.param.gas_const = 287
interactions.aero_drag.param.temp_free_stream = 1487
