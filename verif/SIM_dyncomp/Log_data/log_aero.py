exec(compile(open("Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))

def log_aero ( log_cycle) :
  dr_group = trick.sim_services.DRBinary("aero")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  log_add_pfix_state (dr_group) 

  for ii in range(0,3) :
    dr_group.add_variable( "vehicle.aero_drag.aero_force[" + str(ii) + "]" )
    dr_group.add_variable( "vehicle.aero_drag.aero_torque[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
