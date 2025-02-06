dr_group = trick.sim_services.DRAscii("V_1_State")
dr_group.set_cycle(5400)
dr_group.freq = trick.sim_services.DR_Always
dr_group.add_variable(   "veh.solar_beta.solar_beta")
for ii in range(3) :
  dr_group.add_variable("veh.dyn_body.structure.state.trans.position[" + str(ii) + "]" )

trick.add_data_record_group(dr_group)
