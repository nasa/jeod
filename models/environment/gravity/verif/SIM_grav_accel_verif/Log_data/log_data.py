def log_3_vec( var):
  for ii in range(0,3) :
    dr_group.add_variable(var + "[" + str(ii) + "]" )


dr_group = trick.sim_services.DRBinary("veh_data")
dr_group.set_cycle(1.0)
dr_group.freq = trick.sim_services.DR_Always
log_3_vec("vehicle.dyn_body.composite_body.state.trans.position")
log_3_vec("vehicle.dyn_body.composite_body.state.trans.velocity")
log_3_vec("vehicle.dyn_body.grav_interaction.grav_accel")
dr_group.add_variable("vehicle.dyn_body.grav_interaction.grav_pot")

trick.add_data_record_group(dr_group)
