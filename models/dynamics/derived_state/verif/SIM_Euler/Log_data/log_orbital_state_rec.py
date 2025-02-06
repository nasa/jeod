def log_3_vec( drg, var):
  for ii in range(3):
    drg.add_variable(var+"["+str(ii)+"]")

dr_group = trick.sim_services.DRAscii("V_1_State")
dr_group.set_cycle(12.0)
dr_group.freq = trick.sim_services.DR_Always
trick.add_data_record_group(dr_group)
log_3_vec( dr_group, "veh.euler_rpy.ref_body_angles")
log_3_vec( dr_group, "veh.euler_rpy.body_ref_angles")
log_3_vec( dr_group, "veh.euler_pyr_lvlh.ref_body_angles")
log_3_vec( dr_group, "veh.euler_pyr_lvlh.body_ref_angles")
log_3_vec( dr_group, "veh.euler_rpy_lvlh.ref_body_angles")
log_3_vec( dr_group, "veh.euler_rpy_lvlh.body_ref_angles")
log_3_vec( dr_group, "veh.euler_ypr_lvlh.ref_body_angles")
log_3_vec( dr_group, "veh.euler_ypr_lvlh.body_ref_angles")
log_3_vec( dr_group, "veh.euler_ryp_lvlh.ref_body_angles")
log_3_vec( dr_group, "veh.euler_ryp_lvlh.body_ref_angles")
log_3_vec( dr_group, "veh.euler_yrp_lvlh.ref_body_angles")
log_3_vec( dr_group, "veh.euler_yrp_lvlh.body_ref_angles")
log_3_vec( dr_group, "veh.dyn_body.structure.state.trans.position")

