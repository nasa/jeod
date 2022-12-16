# Set Up Dating Recording
exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))
log_state(1.0)

# Configure the dynamics manager to operate in empty space mode 
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

rk_integrator = trick.RK4IntegratorConstructor()
dynamics.dyn_manager_init.integ_constructor = rk_integrator


vehicleA.body.set_name("vehicleA")
#vehicleA.dyn_body.integ_frame_name = "Space.inertial"
#vehicleA.dyn_body.translational_dynamics = False
#vehicleA.dyn_body.rotational_dynamics = False
vehicleA.tank.set_name("tankA")

vehicleB.body.set_name("vehicleB")
vehicleB.tank.set_name("tankB")

exec(compile(open("Modified_data/vehicle_mass_props.py", "rb").read(), "Modified_data/vehicle_mass_props.py", 'exec'))
mass_props(vehicleA)
mass_props(vehicleB)

exec(compile(open("Modified_data/attach.py", "rb").read(), "Modified_data/attach.py", 'exec'))


trick.add_read(0.0 , """
vehicleA.body.print_tree ( "RUN_test/initialization.out", 100 )
""")

trick.add_read(2.0 , """
vehicleB.tank.core_properties.mass = vehicleB.tank.core_properties.mass * 0.5
vehicleB.tank.set_update_flag()
vehicleA.body.update_mass_properties()
vehicleA.body.print_tree ( "RUN_test/fifty_percent.out", 100 )
""")

trick.add_read(4.0 , """
vehicleB.tank.core_properties.mass = vehicleB.tank.core_properties.mass * 0.4
vehicleB.tank.set_update_flag()
vehicleA.body.update_mass_properties()
vehicleA.body.print_tree ( "RUN_test/eighty-percent.out", 100 )
""")

trick.add_read(6.0 , """
vehicleB.tank.core_properties.mass = vehicleB.tank.core_properties.mass * 0.5
vehicleB.tank.set_update_flag()
vehicleA.body.update_mass_properties()
vehicleA.body.print_tree ( "RUN_test/ninety-percent.out", 100 )
""")

trick.add_read(8.0 , """
vehicleB.tank.core_properties.mass = 0.0
vehicleB.tank.set_update_flag()
vehicleA.body.update_mass_properties()
vehicleA.body.print_tree ( "RUN_test/hundred-percent.out", 100 )
""")

trick.sim_services.exec_set_terminate_time(10.0)
