trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

exec(compile(open( "Log_data/log_orbital_state_rec.py", "rb").read(), "Log_data/log_orbital_state_rec.py", 'exec'))

exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))
exec(compile(open( "Modified_data/veh_config.py", "rb").read(), "Modified_data/veh_config.py", 'exec'))
veh_config(veh)
exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
veh_mass(veh)

exec(compile(open( "Modified_data/veh_state.py", "rb").read(), "Modified_data/veh_state.py", 'exec'))
veh_state(veh)

veh.euler_rpy.sequence = trick.Orientation.Roll_Pitch_Yaw
veh.euler_pyr_lvlh.sequence = trick.Orientation.Pitch_Yaw_Roll
veh.euler_rpy_lvlh.sequence = trick.Orientation.Roll_Pitch_Yaw
veh.euler_ypr_lvlh.sequence = trick.Orientation.Yaw_Pitch_Roll
veh.euler_ryp_lvlh.sequence = trick.Orientation.Roll_Yaw_Pitch
veh.euler_yrp_lvlh.sequence = trick.Orientation.Yaw_Roll_Pitch

veh.lvlh.reference_name = "Earth"

earth.rnp.enable_polar = False 

trick.stop(86400)
