def log_3_vec( drg, var):
  for ii in range(3):
    drg.add_variable(var+"["+str(ii)+"]")

drg1 = trick.sim_services.DRAscii("RelState")
drg1.set_cycle(1)
drg1.freq = trick.sim_services.DR_Always
log_3_vec( drg1, "rel_state.vehA_wrt_vehB_in_B.rel_state.trans.position")
log_3_vec( drg1, "rel_state.vehA_wrt_vehB_in_B.rel_state.trans.velocity")
log_3_vec( drg1, "rel_state.vehA_wrt_vehB_in_B.rel_state.rot.ang_vel_this")

log_3_vec( drg1, "rel_state.vehB_wrt_vehA_in_A.rel_state.trans.position")
log_3_vec( drg1, "rel_state.vehB_wrt_vehA_in_A.rel_state.trans.velocity")
log_3_vec( drg1, "rel_state.vehB_wrt_vehA_in_A.rel_state.rot.ang_vel_this")

log_3_vec( drg1, "rel_state.vehA_wrt_vehB_in_B_generalized.rel_state.trans.position")
log_3_vec( drg1, "rel_state.vehA_wrt_vehB_in_B_generalized.rel_state.trans.velocity")
log_3_vec( drg1, "rel_state.vehA_wrt_vehB_in_B_generalized.rel_state.rot.ang_vel_this")

trick.add_data_record_group(drg1)




drg2 = trick.sim_services.DRAscii("VehState")
drg2.set_cycle(1)
drg2.freq = trick.sim_services.DR_Always

for veh in ["vehA", "vehB"]:
  for frm in ["composite_body", "structure"]:
    log_3_vec( drg2, veh+".dyn_body."+frm+".state.trans.position")
    log_3_vec( drg2, veh+".dyn_body."+frm+".state.trans.velocity")
    log_3_vec( drg2, veh+".dyn_body."+frm+".state.rot.ang_vel_this")

trick.add_data_record_group(drg2)
