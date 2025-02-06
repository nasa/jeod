exec(compile(open( "Log_data/log_state.py", "rb").read(), "Log_data/log_state.py", 'exec'))

def log_force_torque ( log_cycle ) :
  dr_group = trick.sim_services.DRBinary("Force_Torque")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  log_add_derivs( dr_group)

  for ii in range(0,3) :
    dr_group.add_variable("vehicle.dyn_body.collect.effector_forc[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.collect.environ_forc[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.collect.no_xmit_forc[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.collect.effector_torq[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.collect.environ_torq[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.collect.no_xmit_torq[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.collect.inertial_torq[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
