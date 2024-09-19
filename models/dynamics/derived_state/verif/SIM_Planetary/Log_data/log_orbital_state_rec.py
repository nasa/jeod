def log_3_vec( drg, var):
  for ii in range(3):
    drg.add_variable(var+"["+str(ii)+"]")


dr_group = trick.sim_services.DRBinary("V_1_State")
dr_group.set_cycle(12)
dr_group.freq = trick.sim_services.DR_Always

log_3_vec( dr_group,   "veh.pfix.state.cart_coords")
dr_group.add_variable( "veh.pfix.state.sphere_coords.altitude")
dr_group.add_variable( "veh.pfix.state.sphere_coords.latitude")
dr_group.add_variable( "veh.pfix.state.sphere_coords.longitude")
dr_group.add_variable( "veh.pfix.state.ellip_coords.altitude")
dr_group.add_variable( "veh.pfix.state.ellip_coords.latitude")
dr_group.add_variable( "veh.pfix.state.ellip_coords.longitude")
log_3_vec( dr_group,   "veh.dyn_body.structure.state.trans.position")

trick.add_data_record_group(dr_group)
