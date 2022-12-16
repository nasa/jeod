exec(compile(open("SET_test/input_common.py", "rb").read(), "SET_test/input_common.py", 'exec'))

rk_integrator = trick.RK4IntegratorConstructor()
#gj_integrator.thisown = 0
#gj_integrator.order = 8
#gj_integrator.max_rk4_step= -1.0  # bootstrap off
#gj_integrator.perform_convergence_test = False

jeod_time.time_manager.dyn_time.scale_factor = 100

#gj_integrator.convergence_criterioni = 1E-9   DEFAULT SETTING
#gj_integrator.max_corrections_iterations = 10 DEFAULT SETTING


dynamics.dyn_manager_init.integ_constructor = rk_integrator

log_data_rec(300.0/jeod_time.time_manager.dyn_time.scale_factor)
trick.sim_services.exec_set_terminate_time(300000/jeod_time.time_manager.dyn_time.scale_factor)
