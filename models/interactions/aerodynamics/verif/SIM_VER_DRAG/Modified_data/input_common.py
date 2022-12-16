# Set Up Dating Recording
exec(compile(open("Log_data/log_drag_rec.py", "rb").read(), "Log_data/log_drag_rec.py", 'exec'))

aero_test.aero_drag.param.gas_const = 287
aero_test.aero_drag.param.temp_free_stream = 1487
aero_test.aero_drag.active = True

aero_test.center_grav = [0,0,0]
aero_test.atmos_state.wind = [0,0,0]
aero_test.atmos_state.density = 1.0E-12
aero_test.mass = 1.0

aero_test.T_inertial_struct = [ [1,0,0],\
                                [0,1,0],\
                                [0,0,1] \
                              ]

aero_test.aero_drag.use_default_behavior = False
aero_test.aero_drag.set_aero_surface(aero_test.aero_surface)

trick.stop(360.0)
