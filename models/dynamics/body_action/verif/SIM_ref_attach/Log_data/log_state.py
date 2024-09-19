def log_add_state( drg, frame_state):
  for ii in range(0,3) :
    drg.add_variable( frame_state + ".trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    drg.add_variable( frame_state + ".trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    drg.add_variable( frame_state + ".rot.ang_vel_this[" + str(ii) + "]" )


def log_rel_state_rec ( log_cycle ) :
  dr_group = trick.sim_services.DRBinary("RelState")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  log_add_state( dr_group, "rel_state.target_in_pfix.rel_state")

  trick.add_data_record_group(dr_group)



def log_veh_state_rec ( log_cycle ) :
  dr_group = trick.sim_services.DRBinary("VehState")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always

  log_add_state( dr_group, "target.dyn_body.composite_body.state")
  log_add_state( dr_group, "target.dyn_body.structure.state")

  trick.add_data_record_group(dr_group)
