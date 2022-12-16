exec(compile(open("SET_test/input_common.py", "rb").read(), "SET_test/input_common.py", 'exec'))

gj_integrator = trick.GaussJacksonIntegratorConstructor()
gj_integrator.thisown = 0 

gj_config = trick.GaussJacksonConfig.default_configuration()
gj_config.thisown = 0 

gj_config.initial_order = 8 
gj_config.final_order = 8 
gj_config.ndoubling_steps = 0 
gj_config.max_correction_iterations = 1 

gj_integrator.configure (gj_config)


jeod_time.time_manager.dyn_time.scale_factor = 100


dynamics.dyn_manager_init.integ_constructor = gj_integrator

log_data_rec(300.0/jeod_time.time_manager.dyn_time.scale_factor)
trick.sim_services.exec_set_terminate_time(300000/jeod_time.time_manager.dyn_time.scale_factor)
