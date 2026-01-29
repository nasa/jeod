def log_constants ( log_cycle ) :
  dr_group = trick.sim_services.DRBinary("constants")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable( "earth.gravity_source.mu")
  dr_group.add_variable( "earth.planet.r_eq")
  dr_group.add_variable( "earth.planet.r_pol")
  dr_group.add_variable( "earth.planet.e_ellipsoid")
  dr_group.add_variable( "earth.planet.e_ellip_sq")
  dr_group.add_variable( "earth.planet.flat_coeff")
  dr_group.add_variable( "earth.planet.flat_inv")
  dr_group.add_variable( "earth.rnp.planet_omega")
  dr_group.add_variable( "earth.gravity_source.mu")
  dr_group.add_variable( "earth.gravity_source.radius")
  dr_group.add_variable( "sun.gravity_source.mu")
  dr_group.add_variable( "moon.gravity_source.mu")

  trick.add_data_record_group(dr_group)
