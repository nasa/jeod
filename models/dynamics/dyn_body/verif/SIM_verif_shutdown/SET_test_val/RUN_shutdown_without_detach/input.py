"""
"  Executive
"""
jeod_sys.interface_init.message_suppression_level = trick.MessageHandler.Debug
jeod_time.time_manager_init.initializer = "Dyn"

dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "central_point"

trick.sim_services.exec_set_terminate_time(30)


exec(open('Modified_data/veh.py').read())
exec(open('Modified_data/payload.py').read())
exec(open('Modified_data/attach.py').read())

