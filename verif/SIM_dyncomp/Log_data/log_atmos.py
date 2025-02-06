exec(compile(open( "Log_data/log_time.py", "rb").read(), "Log_data/log_time.py", 'exec'))

def log_atmos ( log_cycle):
  dr_group = trick.sim_services.DRBinary("atmos")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  log_add_utc_calendar( dr_group)

  dr_group.add_variable(   "earth.atmos.state.exo_temp")
  dr_group.add_variable(   "earth.atmos.state.log10_dens")
  dr_group.add_variable(   "earth.atmos.state.mol_weight")
  dr_group.add_variable(   "vehicle.atmos_state.density")
  dr_group.add_variable(   "vehicle.atmos_state.temperature")
  dr_group.add_variable(   "vehicle.atmos_state.pressure")
  for ii in range(0,3) :
    dr_group.add_variable( "vehicle.atmos_state.wind[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
