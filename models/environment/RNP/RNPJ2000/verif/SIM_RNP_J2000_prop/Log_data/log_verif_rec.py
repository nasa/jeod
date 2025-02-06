def log_rnp_verif (log_cycle):
  drg = trick.sim_services.DRBinary("rnp_test")
  drg.thisown = False
  drg.set_freq(trick.DR_Always)
  drg.set_cycle(log_cycle)

  drg.add_variable("earth.rnp.RJ2000.theta_gast")
  drg.add_variable("earth.rnp.NJ2000.equa_of_equi")
  drg.add_variable("earth.rnp.enable_polar")
  drg.add_variable("earth.rnp.PMJ2000.override_table")
  drg.add_variable("earth.rnp.planet_omega")
  drg.add_variable("earth.rnp.rnp_type")

  for ii in range(0,3) :
    drg.add_variable("earth.output_vector[" + str(ii) + "]")
  for ii in range(0,3) :
    for jj in range(0,3) :
      var = "earth.logging.nut_trans[" + str(ii) + "][" + str(jj) + "]"
      drg.add_variable(var)
  for ii in range(0,3) :
    for jj in range(0,3) :
      var = "earth.logging.prec_trans[" + str(ii) + "][" + str(jj) + "]"
      drg.add_variable(var)
  for ii in range(0,3) :
    for jj in range(0,3) :
      var = "earth.logging.rot_trans[" + str(ii) + "][" + str(jj) + "]"
      drg.add_variable(var)
  for ii in range(0,3) :
    for jj in range(0,3) :
      var = "earth.planet.pfix.state.rot.T_parent_this[" + str(ii) + "][" + str(jj) + "]"
      drg.add_variable(var)
  for ii in range(0,3) :
    for jj in range(0,3) :
      var = "earth.rnp.NP_matrix[" + str(ii) + "][" + str(jj) + "]"
      drg.add_variable(var)

  # Add the data recording group to Trick's data recording.
  trick.add_data_record_group(drg, trick.DR_Buffer)
