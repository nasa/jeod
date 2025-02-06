#Auto-generated from the data file with this header:
#/*
#PURPOSE:
#    (Log data for planetary ephemeris data.)
#
#REFERENCES:
#    (None)
#
#ASSUMPTIONS AND LIMITATIONS:
#    (None)
#
# 
#*/

def log_planetary_ephem_rec ( log_cycle ) :
  recording_group_name =  "Planetary_Ephem"
  dr_group = trick.sim_services.DRBinary(recording_group_name)
  dr_group.thisown = 0
  dr_group.set_cycle(log_cycle)
  dr_group.freq = trick.sim_services.DR_Always
  dr_group.add_variable(   "jeod_time.tt.trunc_julian_time")
  for ii in range(0,3) :
    dr_group.add_variable("sun.planet.inertial.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("sun.planet.inertial.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.planet.inertial.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("earth.planet.inertial.state.trans.velocity[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("moon.planet.inertial.state.trans.position[" + str(ii) + "]" )
  for ii in range(0,3) :
    dr_group.add_variable("moon.planet.inertial.state.trans.velocity[" + str(ii) + "]" )

  trick.add_data_record_group(dr_group)
  return
