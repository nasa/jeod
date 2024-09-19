trick.sim_services.exec_set_trap_sigfpe(1)
dynamics.dyn_manager_init.sim_integ_opt = trick.sim_services.Runge_Kutta_4

exec(compile(open( "Log_data/log_orbital_state_rec.py", "rb").read(), "Log_data/log_orbital_state_rec.py", 'exec'))

exec(compile(open( "Modified_data/date_and_time.py", "rb").read(), "Modified_data/date_and_time.py", 'exec'))

exec(compile(open( "Modified_data/veh_config.py", "rb").read(), "Modified_data/veh_config.py", 'exec'))
veh_config( veh)
veh.grav_control_sun.source_name = "Sun"
veh.grav_control_sun.active      = False
veh.grav_control_sun.spherical   = True
veh.dyn_body.grav_interaction.add_control(veh.grav_control_sun)

veh.grav_control_moon.source_name = "Moon"
veh.grav_control_moon.active      = False
veh.grav_control_moon.spherical   = True
veh.dyn_body.grav_interaction.add_control(veh.grav_control_moon)

veh.solar_beta.reference_name = "Earth"

exec(compile(open( "Modified_data/veh_mass.py", "rb").read(), "Modified_data/veh_mass.py", 'exec'))
veh_mass(veh)

earth.rnp.enable_polar = False

#trick.stop(31536000)
trick.stop(864000)


def use_trans_init():
  veh.trans_init.set_subject_body( veh.dyn_body )
  veh.trans_init.reference_ref_frame_name = "Earth.inertial"
  veh.trans_init.body_frame_id     = "composite_body"
  veh.trans_init.position  = [ 0.0, 0.0, 0.0]
  veh.trans_init.velocity  = [ 0.0, 0.0, 0.0]
  dynamics.dyn_manager.add_body_action(veh.trans_init)

def use_orb_init():
  veh.init_orbit.set_subject_body( veh.dyn_body )
  veh.init_orbit.reference_ref_frame_name = "Earth.inertial"
  veh.init_orbit.orbit_frame_name = "Earth.inertial"
  veh.init_orbit.planet_name = "Earth"
  veh.init_orbit.body_frame_id = "composite_body"
  veh.init_orbit.set = trick.DynBodyInitOrbit.IncAscnodeAltperAltapoArgperTanom #4
  veh.init_orbit.ascending_node = trick.attach_units("degree", -100.1800767604301)
  veh.init_orbit.true_anomaly   = trick.attach_units("degree", 23.42730924556486)
  veh.init_orbit.inclination    = trick.attach_units("degree", 23.440504499999989)
  veh.init_orbit.alt_apoapsis   = trick.attach_units( "km",400.0)
  veh.init_orbit.alt_periapsis  = trick.attach_units( "km",400.0)
  dynamics.dyn_manager.add_body_action (veh.init_orbit)

def use_rot_init():
  veh.rot_init.set_subject_body( veh.dyn_body )
  veh.rot_init.reference_ref_frame_name = veh.body.integ_frame_name
  veh.rot_init.body_frame_id            = "composite_body"
  veh.rot_init.orientation.data_source  = trick.Orientation.InputQuaternion
  veh.rot_init.ang_velocity             = [ 0.0, 0.0, 0.0]
  dynamics.dyn_manager.add_body_action(veh.rot_init)

def use_lvlh_rot_init():
  veh.init_lvlh.planet_name = "Earth"
  veh.init_lvlh.set_subject_body( veh.dyn_body )
  veh.init_lvlh.reference_ref_frame_name = "Earth.inertial"
  veh.init_lvlh.body_frame_id = "composite_body"

  veh.init_lvlh.orientation.data_source = trick.Orientation.InputEulerRotation
  veh.init_lvlh.orientation.euler_sequence = trick.Orientation.Pitch_Yaw_Roll
  veh.init_lvlh.orientation.euler_angles  = \
                                   trick.attach_units( "degree",[ 30.0, 0.0, 0.0])
  veh.init_lvlh.ang_velocity = [ 0.0, 0.0, 0.0]

  dynamics.dyn_manager.add_body_action (veh.init_lvlh)
