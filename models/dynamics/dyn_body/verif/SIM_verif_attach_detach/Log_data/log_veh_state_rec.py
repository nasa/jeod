def log_3_vec( drg, var):
  for ii in range (3):
    drg.add_variable( var + "[" + str(ii) + "]")

def log_3_vehicles_3_vec( drg, var):
  for ii in range(1,4) :
    log_3_vec( drg, "veh"+ str(ii) + "." + var)

recording_group_name =  "vehicle_State"
dr_group = trick.sim_services.DRBinary("vehicle_State")
dr_group.set_cycle(0.05)
dr_group.freq = trick.sim_services.DR_Always
trick.add_data_record_group(dr_group)

dr_group.add_variable(   "veh1.dyn_body.mass.composite_properties.mass")
dr_group.add_variable(   "veh2.dyn_body.mass.composite_properties.mass")
dr_group.add_variable(   "veh3.dyn_body.mass.composite_properties.mass")
log_3_vehicles_3_vec(dr_group, "dyn_body.composite_body.state.rot.ang_vel_this")
log_3_vehicles_3_vec(dr_group, "dyn_body.composite_body.state.trans.position")
log_3_vehicles_3_vec(dr_group, "dyn_body.composite_body.state.trans.velocity")
log_3_vehicles_3_vec(dr_group, "dyn_body.core_body.state.trans.position")
log_3_vehicles_3_vec(dr_group, "dyn_body.core_body.state.trans.velocity")
log_3_vehicles_3_vec(dr_group, "dyn_body.derivs.trans_accel")
log_3_vehicles_3_vec(dr_group, "dyn_body.derivs.non_grav_accel")
log_3_vehicles_3_vec(dr_group, "dyn_body.derivs.rot_accel")
