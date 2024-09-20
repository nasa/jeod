#PURPOSE:
#  (To define the input data for simulation SET_test/RUN_simple_attach_detach

trick.sim_services.exec_set_trap_sigfpe(1)

exec(compile(open( "Log_data/log_veh_state_rec.py", "rb").read(), "Log_data/log_veh_state_rec.py", 'exec'))

jeod_sys.interface_init.message_suppression_level = trick.MessageHandler.Debug
jeod_time.time_manager_init.initializer = "Dyn"

dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "central_point"


exec(compile(open("Modified_data/veh1.py", "rb").read(), "Modified_data/veh1.py", 'exec'))
exec(compile(open("Modified_data/veh2.py", "rb").read(), "Modified_data/veh2.py", 'exec'))
exec(compile(open("Modified_data/veh3.py", "rb").read(), "Modified_data/veh3.py", 'exec'))
veh3.rot_init.ang_velocity = [ 0.0, 0.0, 0.0]

exec(compile(open("Modified_data/attach_detach.py", "rb").read(), "Modified_data/attach_detach.py", 'exec'))


trick.add_read( 10    , "veh1.attach_to_2.active = True")
trick.add_read( 20    , "veh1.detach_from_2.active = True")
trick.add_read( 30    , '''
veh1.dyn_body.set_attitude_rate([0.0, 1.0, 0.0], veh1.dyn_body.composite_body)
''')
trick.add_read( 35    , '''
attach_to_frame_helper.rotation[0][0] = -1.0
attach_to_frame_helper.rotation[1][1] = -1.0
attach_to_frame_helper.attach_wrap_pos_rot_parent(veh1.dyn_body.find_vehicle_point("node13"))
''')
trick.add_read( 40    , 'veh1.dyn_body.attach_to_frame("central_point.inertial")')
trick.add_read( 50    , '''
veh1.dyn_body.detach()
attach_to_frame_helper.rotation[0][0] = 1.0
attach_to_frame_helper.rotation[1][1] = 1.0
attach_to_frame_helper.attach_wrap_child_parent_pos_rot("veh1.node12", "central_point.inertial")
''')

trick.sim_services.exec_set_terminate_time(60)
