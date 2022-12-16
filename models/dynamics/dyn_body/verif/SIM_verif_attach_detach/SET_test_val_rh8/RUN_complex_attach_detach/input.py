#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_complex_attach_detach/

trick.sim_services.exec_set_trap_sigfpe(1)

exec(compile(open( "Log_data/log_veh_state_rec.py", "rb").read(), "Log_data/log_veh_state_rec.py", 'exec'))

jeod_sys.interface_init.message_suppression_level = 9999
jeod_time.time_manager_init.initializer = "Dyn"

dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "central_point"


exec(compile(open("Modified_data/veh1.py", "rb").read(), "Modified_data/veh1.py", 'exec'))
exec(compile(open("Modified_data/veh2.py", "rb").read(), "Modified_data/veh2.py", 'exec'))
exec(compile(open("Modified_data/veh3.py", "rb").read(), "Modified_data/veh3.py", 'exec'))

exec(compile(open("Modified_data/attach_detach.py", "rb").read(), "Modified_data/attach_detach.py", 'exec'))


trick.add_read( 10    , "veh1.attach_to_2.active = True")
trick.add_read( 32.777, "veh1.attach_to_3.active = True")
trick.add_read( 50    , "veh1.detach_from_2.active = True")
trick.add_read( 55    , "veh1.attach_to_2b.active = True")
trick.add_read( 60    , "veh1.detach_from_3.active = True")

trick.stop(65)
