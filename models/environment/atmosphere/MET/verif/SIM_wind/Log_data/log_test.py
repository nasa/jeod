dr_group = trick.sim_services.DRBinary("test_data")
dr_group.thisown = 0
dr_group.set_cycle(1.0)
dr_group.freq = trick.sim_services.DR_Always

dr_group.add_variable("earth.altitude")
for ii in range(0,3) :
  dr_group.add_variable("earth.wind_vel[" + str(ii) + "]" )

trick.add_data_record_group(dr_group)
