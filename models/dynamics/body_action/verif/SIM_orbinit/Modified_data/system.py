################################################################################
# Provide default Trick settings.
################################################################################

def set_system() :

  # Enable signal handlers.
  trick.sim_services.exec_set_trap_sigbus(1)
  trick.sim_services.exec_set_trap_sigfpe(1)
  trick.sim_services.exec_set_trap_sigsegv(1)

  # Turn off unneeded Trick objects.
  trick_sys.sched.set_sim_object_onoff("trick_vs", 0)
  trick_sys.sched.set_sim_object_onoff("instruments", 0)
  trick_sys.sched.set_sim_object_onoff("utest", 0)
  trick.set_var_server_enabled(0)
  # trick.message_device_set_enabled(0)
