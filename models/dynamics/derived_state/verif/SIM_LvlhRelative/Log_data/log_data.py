def log_3_vec( drg, var):
  for ii in range(3):
    drg.add_variable(var+"["+str(ii)+"]")

def log_3x3_mx( drg, var):
  for ii in range(3):
    log_3_vec( drg, var+"["+str(ii)+"]")

drg1 = trick.sim_services.DRBinary("RelState")
drg1.set_cycle(1)
drg1.freq = trick.sim_services.DR_Always

for rs in ["vehA_wrt_vehB_in_B",
           "vehB_wrt_vehA_in_A",
           "vehA_in_vehB_rectilvlh",
           "vehB_in_vehA_rectilvlh",
           "vehA_in_vehB_curvilvlh",
           "vehB_in_vehA_curvilvlh",
           "test_c2r"]:
  log_3_vec( drg1, "rel_state." + rs + ".rel_state.trans.position")
  log_3_vec( drg1, "rel_state." + rs + ".rel_state.trans.velocity")
  log_3_vec( drg1, "rel_state." + rs + ".rel_state.rot.ang_vel_this")
  log_3x3_mx(drg1, "rel_state." + rs + ".rel_state.rot.T_parent_this")

drg1.add_variable("rel_state.vehA_in_vehB_curvilvlh.use_theta_dot_correction")
drg1.add_variable("rel_state.vehB_in_vehA_curvilvlh.use_theta_dot_correction")

trick.add_data_record_group(drg1)


drg2 = trick.sim_services.DRBinary("VehState")
drg2.set_cycle(1)
drg2.freq = trick.sim_services.DR_Always

for veh in ["vehA", "vehB"]:
  log_3_vec( drg2, veh+".dyn_body.composite_body.state.trans.position")
  log_3_vec( drg2, veh+".dyn_body.composite_body.state.trans.velocity")
  log_3_vec( drg2, veh+".dyn_body.composite_body.state.rot.ang_vel_this")

trick.add_data_record_group(drg2)
