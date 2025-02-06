exec(compile(open( "Log_data/log_sv_orbelem_rec.py", "rb").read(), "Log_data/log_sv_orbelem_rec.py", 'exec'))
exec(compile(open( "Log_data/log_sv_state_rec.py", "rb").read(), "Log_data/log_sv_state_rec.py", 'exec'))
exec(compile(open( "Modified_data/rnp.py", "rb").read(), "Modified_data/rnp.py", 'exec'))
exec(compile(open( "Modified_data/solar.py", "rb").read(), "Modified_data/solar.py", 'exec'))
exec(compile(open( "Modified_data/radiation_pressure.py", "rb").read(), "Modified_data/radiation_pressure.py", 'exec'))
exec(compile(open( "Modified_data/mass.py", "rb").read(), "Modified_data/mass.py", 'exec'))
exec(compile(open( "Modified_data/state.py", "rb").read(), "Modified_data/state.py", 'exec'))
exec(compile(open( "Modified_data/aero.py", "rb").read(), "Modified_data/aero.py", 'exec'))


def unique_input (cycle, name):
  #/* Set the vehicle name. */
  vehicle.dyn_body.set_name( name )

  log_sv_orbelem_rec( cycle , name )
  log_sv_state_rec( cycle , name )

  set_rnp( name)
  set_solar(name)
  set_rad_pressure(name)
  set_mass(name)
  set_state(name)
  set_aero(name)


def earth_grav(degree, order) :
  vehicle.earth_grav_control.degree      = degree
  vehicle.earth_grav_control.order       = order



trick.sim_services.exec_set_trap_sigfpe(1)

#/* Control ephemeris update. */
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4
dynamics.dyn_manager.deriv_ephem_update = True

#/* Configure vehicle integration information. */
vehicle.dyn_body.integ_frame_name = "Earth.inertial"
vehicle.dyn_body.translational_dynamics = True
vehicle.dyn_body.rotational_dynamics = True

#/* Configure the dervied reference frames. */
vehicle.pfix.reference_name     = "Earth"
vehicle.lvlh.reference_name     = "Earth"
vehicle.orb_elem.reference_name = "Earth"

#/* Configure the atmosphere. */
vehicle.atmos_state.active = True


dynamics.dyn_manager.add_body_action(vehicle.mass_init)
dynamics.dyn_manager.add_body_action(vehicle.trans_init)
dynamics.dyn_manager.add_body_action(vehicle.lvlh_init)

# Gravity controls
vehicle.earth_grav_control.source_name = "Earth"
vehicle.earth_grav_control.active      = True
vehicle.earth_grav_control.spherical   = False
vehicle.earth_grav_control.gradient    = False

vehicle.moon_grav_control.source_name = "Moon"
vehicle.moon_grav_control.active      = True
vehicle.moon_grav_control.spherical   = True
vehicle.moon_grav_control.gradient    = False

vehicle.sun_grav_control.source_name = "Sun"
vehicle.sun_grav_control.active      = True
vehicle.sun_grav_control.spherical   = True
vehicle.sun_grav_control.gradient    = False

vehicle.dyn_body.grav_interaction.add_control(vehicle.earth_grav_control)
vehicle.dyn_body.grav_interaction.add_control(vehicle.moon_grav_control)
vehicle.dyn_body.grav_interaction.add_control(vehicle.sun_grav_control)



#/* Use this time for regression testing. */
trick.sim_services.exec_set_terminate_time(86400)

trick.sim_services.exec_set_terminate_time(3600)
