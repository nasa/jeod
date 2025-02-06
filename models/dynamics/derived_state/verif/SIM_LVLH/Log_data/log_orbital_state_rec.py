def log_3_vec( drg, var):
  for ii in range(3):
    drg.add_variable(var+"["+str(ii)+"]")

def log_3x3_mx( drg, var):
  for ii in range(3):
    log_3_vec( drg, var+"["+str(ii)+"]")

dr_group = trick.sim_services.DRAscii("V_1_State")
dr_group.set_cycle(12)
dr_group.freq = trick.sim_services.DR_Always
log_3_vec( dr_group, "rel_state.vehA_wrt_vehB.rel_state.trans.position")
log_3_vec( dr_group, "rel_state.vehA_wrt_vehB.rel_state.trans.velocity")
log_3_vec( dr_group, "rel_state.vehB_wrt_vehA.rel_state.trans.position")
log_3_vec( dr_group, "rel_state.vehB_wrt_vehA.rel_state.trans.velocity")
for veh in ["vehA", "vehB"]:
  log_3x3_mx( dr_group,  veh+".lvlh.lvlh_frame.state.rot.T_parent_this")
  dr_group.add_variable( veh+".lvlh.lvlh_frame.state.rot.ang_vel_mag")
for veh in ["vehA", "vehB"]:
  log_3_vec( dr_group,   veh+".dyn_body.composite_body.state.trans.position")
  log_3_vec( dr_group,   veh+".dyn_body.composite_body.state.trans.velocity")

trick.add_data_record_group(dr_group)
