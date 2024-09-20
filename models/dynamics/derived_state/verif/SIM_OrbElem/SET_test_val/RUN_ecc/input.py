trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

exec(compile(open( "Log_data/log_orbital_state_rec.py", "rb").read(), "Log_data/log_orbital_state_rec.py", 'exec'))

exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))

exec(compile(open( "Modified_data/veh_config.py", "rb").read(), "Modified_data/veh_config.py", 'exec'))
veh_config( veh)

exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
veh_mass( veh)

exec(compile(open( "Modified_data/veh_state.py", "rb").read(), "Modified_data/veh_state.py", 'exec'))
veh_state( veh)
veh.init_orbit.alt_periapsis  = trick.attach_units( "km",8000.0)

veh.orb_elem.reference_name = "Earth"

earth.rnp.enable_polar = False 

trick.stop(86400)
