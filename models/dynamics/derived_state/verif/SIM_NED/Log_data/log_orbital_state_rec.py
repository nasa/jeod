def log_3_vec( drg, var):
  for ii in range(3):
    drg.add_variable(var+"["+str(ii)+"]")

dr_group = trick.sim_services.DRBinary("V_1_State")
dr_group.set_cycle(12)
dr_group.freq = trick.sim_services.DR_Always

for veh in ["vehA","vehB"]:
  log_3_vec( dr_group, veh+".ned.ned_state.cart_coords")
  for coords in ["sphere", "ellip"]:
    dr_group.add_variable( veh +".ned.ned_state."+coords+"_coords.altitude")
    dr_group.add_variable( veh +".ned.ned_state."+coords+"_coords.latitude")
    dr_group.add_variable( veh +".ned.ned_state."+coords+"_coords.longitude")

log_3_vec( dr_group, "rel_state.vehA_wrt_vehB.rel_state.trans.position")
log_3_vec( dr_group, "rel_state.vehA_wrt_vehB.rel_state.trans.velocity")
log_3_vec( dr_group, "rel_state.vehB_wrt_vehA.rel_state.trans.position")
log_3_vec( dr_group, "rel_state.vehB_wrt_vehA.rel_state.trans.velocity")
log_3_vec( dr_group, "vehA.dyn_body.structure.state.trans.position")

trick.add_data_record_group(dr_group)
