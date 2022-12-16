dr_group = trick.sim_services.DRBinary("V_1_State")
dr_group.thisown = 0
dr_group.set_cycle(0.5)
dr_group.freq = trick.sim_services.DR_Always
for ii in range(0,3) :
  dr_group.add_variable("veh.dyn_body.composite_body.state.trans.position[" + str(ii) + "]" )
for ii in range(0,3) :
  dr_group.add_variable("veh.dyn_body.composite_body.state.trans.velocity[" + str(ii) + "]" )
for ii in range(0,3) :
  dr_group.add_variable("veh.dyn_body.grav_interaction.grav_accel[" + str(ii) + "]" )
for ii in range(0,3) :
  dr_group.add_variable("veh.dyn_body.derivs.trans_accel[" + str(ii) + "]" )
for ii in range(0,3) :
  dr_group.add_variable("veh.dyn_body.derivs.non_grav_accel[" + str(ii) + "]" )
for ii in range(0,3) :
  dr_group.add_variable("veh.embary.rel_state.trans.position[" + str(ii) + "]" )
for ii in range(0,3) :
  dr_group.add_variable("veh.embary.rel_state.trans.velocity[" + str(ii) + "]" )

trick.add_data_record_group(dr_group)
