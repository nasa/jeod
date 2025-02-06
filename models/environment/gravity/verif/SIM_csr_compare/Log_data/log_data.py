dr_group = trick.sim_services.DRAscii("gravity_compare")
dr_group.set_cycle(1.0)
dr_group.freq = trick.sim_services.DR_Always
trick.add_data_record_group(dr_group)

dr_group.add_variable(   "vehicle.dyn_body.grav_interaction.grav_pot")
for ii in range(3) :
  dr_group.add_variable("vehicle.dyn_body.grav_interaction.grav_accel[" + str(ii) + "]" )
for ii in range(3) :
  dr_group.add_variable("vehicle.dyn_body.composite_body.state.trans.position[" + str(ii) + "]" )
