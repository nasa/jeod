def log_add_earth_RNP( dr_group) :
  dr_group.add_variable(   "earth.rnp.NJ2000.nutation_in_longitude")
  dr_group.add_variable(   "earth.rnp.NJ2000.nutation_in_obliquity")
  dr_group.add_variable(   "earth.rnp.NJ2000.equa_of_equi")
  dr_group.add_variable(   "earth.rnp.RJ2000.theta_gast")
  for ii in range(0,3) :
    for jj in range(0,3) :
      dr_group.add_variable("earth.planet.pfix.state.rot.T_parent_this[" +str(ii) + "][" + str(jj) + "]" )
      dr_group.add_variable("earth.rnp.RJ2000.rotation[" + str(ii) + "][" + str(jj) + "]" )
      dr_group.add_variable("earth.rnp.NJ2000.rotation[" + str(ii) + "][" + str(jj) + "]" )
      dr_group.add_variable("earth.rnp.PJ2000.rotation[" + str(ii) + "][" + str(jj) + "]" )


def log_earth_RNP ( log_cycle ) :
  recording_group_name =  "Earth_RNP"
  dr_group = trick.sim_services.DRBinary("Earth_RNP")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  log_add_earth_RNP( dr_group)
  trick.add_data_record_group(dr_group)

def log_earth_RNP_ascii ( log_cycle ) :
  recording_group_name =  "Earth_RNP"
  dr_group = trick.sim_services.DRAscii("Earth_RNP_ascii")
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  log_add_earth_RNP( dr_group)
  trick.add_data_record_group(dr_group)
