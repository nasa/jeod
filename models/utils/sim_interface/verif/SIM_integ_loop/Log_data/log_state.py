def log_state ( log_cycle ) :
  recording_group_name = "state" 
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  
  dr_group.add_variable( "veh_hi_rate.dyn_body.composite_body.state.trans.position[0]")
  dr_group.add_variable( "veh_hi_rate.dyn_body.composite_body.state.trans.velocity[0]")
  dr_group.add_variable( "veh_hi_med_rate.dyn_body.composite_body.state.trans.position[0]")
  dr_group.add_variable( "veh_hi_med_rate.dyn_body.composite_body.state.trans.velocity[0]")
  dr_group.add_variable( "veh_med_rate.dyn_body.composite_body.state.trans.position[0]")
  dr_group.add_variable( "veh_med_rate.dyn_body.composite_body.state.trans.velocity[0]")
  dr_group.add_variable( "veh_med_lo_rate.dyn_body.composite_body.state.trans.position[0]")
  dr_group.add_variable( "veh_med_lo_rate.dyn_body.composite_body.state.trans.velocity[0]")
  dr_group.add_variable( "veh_lo_rate.dyn_body.composite_body.state.trans.position[0]")
  dr_group.add_variable( "veh_lo_rate.dyn_body.composite_body.state.trans.velocity[0]")

  trick.add_data_record_group(dr_group)

  return
