trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager_init.mode= trick.DynManagerInit.EphemerisMode_Ephemerides
dynamics.dyn_manager.deriv_ephem_update = True

dawn.dyn_body.integ_frame_name = "Mars.inertial"
dawn.dyn_body.translational_dynamics = True
dawn.dyn_body.rotational_dynamics = True

# Turn off Mars RNP polar motion.
mars.rnp.enable_polar = False

# Configure the derived reference frames.
dawn.orb_elem.reference_name = "Mars"
dawn.lvlh.reference_name     = "Mars"

exec(compile(open( "Modified_data/grav_controls.py", "rb").read(), "Modified_data/grav_controls.py", 'exec'))
exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
exec(compile(open( "Modified_data/veh_state.py", "rb").read(), "Modified_data/veh_state.py", 'exec'))
exec(compile(open( "Modified_data/epoch.py", "rb").read(), "Modified_data/epoch.py", 'exec'))
