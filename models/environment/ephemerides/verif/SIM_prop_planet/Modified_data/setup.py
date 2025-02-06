################################################################################
#PURPOSE:
#  (Configure the simulation.)
################################################################################


def set_setup(init_bodies = False) :

  import trick.sim_services
  import os

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


  # Configure the time model.
  # Start the sim at noon, January 1, 1900.
  jeod_time.time_manager_init.initializer = "UTC"
  jeod_time.time_manager_init.sim_start_format = trick.TimeEnum.calendar

  jeod_time.time_utc.send_warning_pre_1968 = False
  jeod_time.time_utc.calendar_year   = 1900
  jeod_time.time_utc.calendar_month  =    1
  jeod_time.time_utc.calendar_day    =    1
  jeod_time.time_utc.calendar_hour   =   12
  jeod_time.time_utc.calendar_minute =    0
  jeod_time.time_utc.calendar_second =   0.0

  jeod_time.time_tai.initialize_from_name = "UTC"
  jeod_time.time_tt.initialize_from_name  = "TAI"

  jeod_time.time_tai.update_from_name = "Dyn"
  jeod_time.time_utc.update_from_name = "TAI"
  jeod_time.time_tt.update_from_name  = "TAI"


  # Configure the sun, jupiter, and saturn.
  exec(compile(open("Modified_data/sun.py", "rb").read(), "Modified_data/sun.py", 'exec'), globals())
  exec(compile(open("Modified_data/jupiter.py", "rb").read(), "Modified_data/jupiter.py", 'exec'), globals())
  set_jupiter (init_bodies)
  exec(compile(open("Modified_data/saturn.py", "rb").read(), "Modified_data/saturn.py", 'exec'), globals())
  set_saturn (init_bodies)


  # Configure the ephemeris model
  env.de4xx.set_model_number(421)


  # Configure the integrator.
  fast_integ_loop.integ_loop.set_deriv_ephem_update (1)


  # Configure the Trick data logger.
  exec(compile(open("Log_data/log_log_states.py", "rb").read(), "Log_data/log_log_states.py", 'exec'), globals())
  log_log_states(["sun", "jupiter", "saturn"])
