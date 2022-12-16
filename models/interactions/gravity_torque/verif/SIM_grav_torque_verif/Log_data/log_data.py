def log_3_vec( var):
  for ii in range(0,3) :
    dr_group.add_variable(var + "[" + str(ii) + "]" )


dr_group = trick.sim_services.DRAscii("State")
dr_group.set_cycle(1.0)
dr_group.freq = trick.sim_services.DR_Always
log_3_vec("vehicle.grav_torque.torque")
log_3_vec("vehicle.grav_verif.verif_grav_torque")
log_3_vec("vehicle.grav_verif.torque_error")

trick.add_data_record_group(dr_group)
