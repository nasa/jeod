

# Universal gravitational constant, in km^3 / day^2
_G = 4.982174e-10

# Items from the DE405 header

# Length of the DE405 astronomical unit, in kilometers
_AU = 0.149597870691000015e+09

# Earth-moon mass ratio (unitless)
_EMRAT = 0.813005600000000044e+02

# Gravitational parameters for Mencury, Venus, Earth+Moon, Mars,
# Jupiter, Saturn, Uranus, Neptune, and the Sun,
# in units of cubic DE405 astronomical units per square day.
_GM1 = 0.491254745145081187e-10
_GM2 = 0.724345248616270270e-09
_GMB = 0.899701134671249882e-09
_GM4 = 0.954953510577925806e-10
_GM5 = 0.282534590952422643e-06
_GM6 = 0.845971518568065874e-07
_GM7 = 0.129202491678196939e-07
_GM8 = 0.152435890078427628e-07
_GMS = 0.295912208285591095e-03

# Derived values

# Earth and Moon gravitational parameters
_GM3 = _GMB * (_EMRAT / (_EMRAT+1.0))
_GMM = _GMB - _GM3

# One oubic DE405 astronomical unit, in cubic kilometers
_AU3 = _AU * _AU * _AU


def _is_modern_trick () :

    import os
    import os.path

    trick_ver_exec = \
        '/'.join((os.getenv("TRICK_HOME"), 'bin/trick-version'))
    result = os.path.isfile(trick_ver_exec)
    return result


def _get_host_cpu () :

    import os

    command = 'trick-gte' if _is_modern_trick() else 'gte'
    command += ' TRICK_HOST_CPU'
    process = os.popen(command)
    result = process.read()
    try :
        process.close()
    except :
        pass
    return result.rstrip()


def configure_trick (trick, trick_sys) :

    import trick.sim_services

    # Configure the simulation control parameters.
    trick.sim_services.exec_set_software_frame(3600*24)
    trick.sim_services.exec_set_trap_sigbus(1)
    trick.sim_services.exec_set_trap_sigfpe(1)
    trick.sim_services.exec_set_trap_sigsegv(1)

    # Turn off unneeded Trick objects
    trick_sys.sched.set_sim_object_onoff("trick_vs", 0)
    trick_sys.sched.set_sim_object_onoff("instruments", 0)
    trick_sys.sched.set_sim_object_onoff("utest", 0)
    trick.set_var_server_enabled(0)
    # trick.message_device_set_enabled(0)


def configure_time (trick, time, base="TAI") :

    # Configure the time model.
    # Start the sim at noon, January 1, 1900 TDB.
    time.time_manager_init.initializer = "TDB"
    time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

    time.time_tdb.send_warning_pre_1968 = False
    time.time_tdb.calendar_year   = 1600
    time.time_tdb.calendar_month  =    1
    time.time_tdb.calendar_day    =    1
    time.time_tdb.calendar_hour   =   12
    time.time_tdb.calendar_minute =    0
    time.time_tdb.calendar_second =   0.0

    time.time_tai.initialize_from_name = "TDB"
    time.time_tt.initialize_from_name = "TAI"

    if base == "TAI" :
        time.time_tai.update_from_name = "Dyn"
        time.time_tdb.update_from_name  = "TAI"
        time.time_tt.update_from_name  = "TAI"
    elif base == "TT" :
        time.time_tt.update_from_name = "Dyn"
        time.time_tai.update_from_name = "TT"
        time.time_tdb.update_from_name = "TAI"
    else :
        assert (base == "TDB")
        time.time_tdb.update_from_name = "Dyn"
        time.time_tai.update_from_name  = "TDB"
        time.time_tt.update_from_name  = "TAI"

    # Run the sim for 601 years (146: number of leap days).
    trick.sim_services.exec_set_terminate_time(86400*(365*601+146))


def configure_env (env) :
    # Configure the ephemeris model
    env.de4xx.set_model_number(405)


def configure_gravity (trick,
    sun, venus, earth, moon, mars, jupiter, saturn, uranus, neptune) :

    sun.gravity_source.mu = trick.attach_units ("km^3/day^2", _GMS * _AU3)
    venus.gravity_source.mu = trick.attach_units ("km^3/day^2", _GM2 * _AU3)
    earth.gravity_source.mu = trick.attach_units ("km^3/day^2", _GM3 * _AU3)
    moon.gravity_source.mu = trick.attach_units ("km^3/day^2", _GMM * _AU3)
    mars.gravity_source.mu = trick.attach_units ("km^3/day^2", _GM4 * _AU3)
    jupiter.gravity_source.mu = trick.attach_units ("km^3/day^2", _GM5 * _AU3)
    saturn.gravity_source.mu = trick.attach_units ("km^3/day^2", _GM6 * _AU3)
    uranus.gravity_source.mu = trick.attach_units ("km^3/day^2", _GM7 * _AU3)
    neptune.gravity_source.mu = trick.attach_units ("km^3/day^2", _GM8 * _AU3)


def configure_mercury (trick, dynamics, mercury, relativistic_set) :
    mass = _GM1 * _AU3 / _G
    radius = float(mercury.planet.r_eq)
    Ixx = 0.4 * mass * radius**2

    mercury.mass_init.set_subject_body( mercury.prop_planet.body.mass )
    mercury.mass_init.action_name = "mercury.mass_unit"

    mercury.mass_init.properties.pt_orientation.data_source = \
        trick.Orientation.InputEigenRotation
    mercury.mass_init.properties.pt_orientation.eigen_angle = \
        trick.attach_units("degree", 0.0)
    mercury.mass_init.properties.pt_orientation.eigen_axis = [0, 1, 0]

    mercury.mass_init.properties.mass = trick.attach_units("kg", mass)
    mercury.mass_init.properties.position = \
        trick.attach_units("m", [0.0, 0.0, 0.0])
    mercury.mass_init.properties.inertia[0] = \
        trick.attach_units("kg*m2", [Ixx, 0.0, 0.0])
    mercury.mass_init.properties.inertia[1] = \
        trick.attach_units("kg*m2", [0.0, Ixx, 0.0])
    mercury.mass_init.properties.inertia[2] = \
        trick.attach_units("kg*m2", [0.0, 0.0, Ixx])

    dynamics.dyn_manager.add_body_action (mercury.mass_init)

    mercury.rot_init.set_subject_body( mercury.prop_planet.body )
    mercury.rot_init.action_name = "mercury.rot_init"
    mercury.rot_init.reference_ref_frame_name = "SSBary.inertial"
    mercury.rot_init.body_frame_id = "composite_body"
    mercury.rot_init.state_items = trick.DynBodyInitRotState.Both
    mercury.rot_init.orientation.data_source = \
        trick.Orientation.InputEulerRotation
    mercury.rot_init.orientation.euler_sequence = \
        trick.Orientation.EulerZXY
    mercury.rot_init.orientation.euler_angles = \
        trick.attach_units("degree", [0.0, 0.0, 0.0])
    mercury.rot_init.ang_velocity = \
        trick.attach_units("degree/s", [0.0, 0.0, 0.0])

    dynamics.dyn_manager.add_body_action (mercury.rot_init)


    for ii in range(9) :
        mercury.activate_body(ii, ii in relativistic_set)


    mercury.prop_planet.commanded_mode = \
      trick.PropagatedPlanet.TransFromPlanet_RotFromBody
    mercury.prop_planet.body.rotational_dynamics = False
