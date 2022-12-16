#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for earth RNP data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (None)
#
# 
#*/

def log_earth_RNP_rec ( log_cycle ) :
  recording_group_name =  "Earth_RNP"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "earth.rnp.NJ2000.nutation_in_longitude")
  dr_group.add_variable(   "earth.rnp.NJ2000.nutation_in_obliquity")
  dr_group.add_variable(   "earth.rnp.NJ2000.equa_of_equi")
  dr_group.add_variable(   "earth.rnp.RJ2000.theta_gast")
  for ii in range(0,3) :
    dr_group.add_variable("earth.planet.pfix.state.rot.T_parent_this[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.planet.pfix.state.rot.T_parent_this[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.planet.pfix.state.rot.T_parent_this[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.RJ2000.rotation[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.RJ2000.rotation[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.RJ2000.rotation[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.NJ2000.rotation[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.NJ2000.rotation[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.NJ2000.rotation[2][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.PJ2000.rotation[0][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.PJ2000.rotation[1][" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.rnp.PJ2000.rotation[2][" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
