trick.sim_services.exec_set_trap_sigfpe(1)
#/* Enable debug messages. */
jeod_sys.interface_init.message_suppression_level = 1000
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode = trick.DynManagerInit.EphemerisMode_EmptySpace
dynamics.dyn_manager_init.central_point_name = "Space"

exec(compile(open("Modified_data/parent_mass.py", "rb").read(), "Modified_data/parent_mass.py", 'exec'))
exec(compile(open("Modified_data/child1_mass.py", "rb").read(), "Modified_data/child1_mass.py", 'exec'))
exec(compile(open("Modified_data/child2_mass.py", "rb").read(), "Modified_data/child2_mass.py", 'exec'))
exec(compile(open("Modified_data/child3_mass.py", "rb").read(), "Modified_data/child3_mass.py", 'exec'))
exec(compile(open("Modified_data/attach1.py", "rb").read(), "Modified_data/attach1.py", 'exec'))
exec(compile(open("Modified_data/attach2.py", "rb").read(), "Modified_data/attach2.py", 'exec'))
exec(compile(open("Modified_data/attach3.py", "rb").read(), "Modified_data/attach3.py", 'exec'))

trick.stop(0)
