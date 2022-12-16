exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(0.1)

dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator

vehicle.dyn_body.set_name("vehicle")
vehicle.dyn_body.integ_frame_name = "Space.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = True

vehicle.array.set_name("array")

exec(compile(open("Modified_data/vehicle_mass_props_part2.py", "rb").read(), "Modified_data/vehicle_mass_props_part2.py", 'exec'))
exec(compile(open("Modified_data/vehicle_state.py", "rb").read(), "Modified_data/vehicle_state.py", 'exec'))
exec(compile(open("Modified_data/surface_part2.py", "rb").read(), "Modified_data/surface_part2.py", 'exec'))
exec(compile(open("Modified_data/attach.py", "rb").read(), "Modified_data/attach.py", 'exec'))


vehicle.reattachments = trick.alloc_type(4,"jeod::BodyReattach")

vehicle.reattachments[0].subject = vehicle.array
vehicle.reattachments[0].offset_pstr_cstr_pstr = [0,0,0]
vehicle.reattachments[0].pstr_cstr.data_source = trick.Orientation.InputEigenRotation
vehicle.reattachments[0].pstr_cstr.eigen_angle = trick.attach_units( "degree", 90.0 )
vehicle.reattachments[0].pstr_cstr.eigen_axis  = [1.0, 0.0, 0.0]
vehicle.reattachments[0].active = False
dynamics.dyn_manager.add_body_action( vehicle.reattachments[0] )

vehicle.reattachments[1].subject = vehicle.array
vehicle.reattachments[1].offset_pstr_cstr_pstr = [0,0,0]
vehicle.reattachments[1].pstr_cstr.data_source = trick.Orientation.InputEigenRotation
vehicle.reattachments[1].pstr_cstr.eigen_angle = trick.attach_units( "degree", 90.0 )
vehicle.reattachments[1].pstr_cstr.eigen_axis  = [0.0, 1.0, 0.0]
vehicle.reattachments[1].active = False
dynamics.dyn_manager.add_body_action( vehicle.reattachments[1] )

vehicle.reattachments[2].subject = vehicle.array
vehicle.reattachments[2].offset_pstr_cstr_pstr = [0,0,0]
vehicle.reattachments[2].pstr_cstr.data_source = trick.Orientation.InputEigenRotation
vehicle.reattachments[2].pstr_cstr.eigen_angle = trick.attach_units( "degree", 90.0 )
vehicle.reattachments[2].pstr_cstr.eigen_axis  = [0.0, 0.0, 1.0]
vehicle.reattachments[2].active = False
dynamics.dyn_manager.add_body_action( vehicle.reattachments[2] )

vehicle.reattachments[3].subject = vehicle.array
vehicle.reattachments[3].offset_pstr_cstr_pstr = [0,0,0]
vehicle.reattachments[3].pstr_cstr.data_source = trick.Orientation.InputEigenRotation
vehicle.reattachments[3].pstr_cstr.eigen_angle = trick.attach_units( "degree", 180.0 )
vehicle.reattachments[3].pstr_cstr.eigen_axis  = [0.0, 0.0, 1.0]
vehicle.reattachments[3].active = False
dynamics.dyn_manager.add_body_action( vehicle.reattachments[3] )

trick.add_read(2.0 , """
vehicle.reattachments[0].active = True
dynamics.dyn_manager.perform_actions()
""")

trick.add_read(4.0 , """
vehicle.reattachments[1].active = True
dynamics.dyn_manager.perform_actions()
""")

trick.add_read(6.0 , """
vehicle.reattachments[2].active = True
dynamics.dyn_manager.perform_actions()
""")

trick.add_read(8.0 , """
vehicle.reattachments[3].active = True
dynamics.dyn_manager.perform_actions()
""")

trick.exec_set_terminate_time(12)
