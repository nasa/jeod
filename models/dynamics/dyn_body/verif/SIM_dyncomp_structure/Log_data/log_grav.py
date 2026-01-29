def log_grav ( log_cycle ) :
  dr_group = trick.sim_services.DRBinary("gravity")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  for ii in range(0,3) :
    dr_group.add_variable("vehicle.pfix.state.cart_coords[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.dyn_body.grav_interaction.grav_accel[" + str(ii) + "]" )
    dr_group.add_variable("vehicle.grav_torque.torque[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
